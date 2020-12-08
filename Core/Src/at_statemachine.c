#include "at_statemachine.h"
#include "usart.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "user_task.h"
#include "utils.h"

#define AT_TRANSMIT_TIME	200

//reset
int at_reset_action1(void);
action_result at_reset_action2(const char *command_buffer);
//at command
int at_action1(void);
action_result at_action2(const char *command_buffer);
//cpin command sim card query
int at_pin_action1(void);
action_result at_pin_action2(const char *command_buffer);
//creg register cs service
int at_creg_action1(void);
action_result at_creg_action2(const char *command_buffer);
//cgreg register ps service
int at_cgreg_action1(void);
action_result at_cgreg_action2(const char *command_buffer);
//configure parameters of a tcp/ip context
int at_csgp_action1(void);
action_result at_csgp_action2(const char *command_buffer);
//activate a pdp context
int at_act_action1(void);
action_result at_act_action2(const char *command_buffer);
//query a pdp context
int at_query_act_action1(void);
action_result at_query_act_action2(const char *command_buffer);
//configure parameters for https server
int at_httpcfg_context_action1(void);
action_result at_httpcfg_context_action2(const char *command_buffer);

//configure parameters for https header
int at_httpcfg_header_action1(void);
action_result at_httpcfg_header_action2(const char *command_buffer);

//configure url for https server
int at_httpurl_action1(void);
action_result at_httpurl_action2(const char *command_buffer);

//configure the url address
int at_httpurl_add_action1(void);
action_result at_httpurl_add_action2(const char *command_buffer);

//post at command
int at_httppost_action1(void);
action_result at_httppost_action2(const char *command_buffer);

//send token request
int at_httptoken_action1(void);
action_result at_httptoken_action2(const char *command_buffer);

//read token command
int at_readtoken_action1(void);
action_result at_readtoken_action2(const char *command_buffer);

int send_data_action1(void);
action_result send_data_action2(const char *command_buffer);

int state_idel_action1(void);
action_result state_idel_action2(const char *command_buffer);

void send_at_command(char* command);

static nbiot_fsm_state_index_t nbiot_fsm_state_index;
static int current_state_index = 0;

	
//��ǰ״̬, ��һ��״̬, ���Դ���, �ȴ�ʱ��, ����, ����
static const nbiot_fsm_state_t nbiot_state_list[] =
{
	//{STATE_HAL_RESET,				STATE_AT, 	         		1,  3000, 	at_reset_action1, 					at_reset_action2		},
	{STATE_AT,       				STATE_CPIN,				 			3,  3000,		at_action1,         				at_action2         	},
	{STATE_CPIN,     				STATE_CREG,	 						3,  10000,	at_pin_action1,       			at_pin_action2     	},
	{STATE_CREG,						STATE_CGREG,		 				3,  30000,	at_creg_action1,  					at_creg_action2  		},
	{STATE_CGREG,						STATE_QICSGP,		 				3,  30000,	at_cgreg_action1, 	 				at_cgreg_action2  	},
	{STATE_QICSGP,					STATE_QIACT,		 				3,  1000,		at_csgp_action1,  					at_csgp_action2  		},
	{STATE_QIACT,						STATE_QIACT_RESULT,		 	3,  1000,		at_act_action1,  						at_act_action2  		},
	{STATE_QIACT_RESULT,		STATE_QHTTPCFG_CONTEXT,	3,  1000,		at_query_act_action1, 			at_query_act_action2},
	{STATE_QHTTPCFG_CONTEXT,STATE_QHTTPREQ_HEADER,	5,  3000,		at_httpcfg_context_action1, at_httpcfg_context_action2},
	{STATE_QHTTPREQ_HEADER,	STATE_QHTTPURL,					5,  3000,		at_httpcfg_header_action1, 	at_httpcfg_header_action2},
	{STATE_QHTTPURL,				STATE_QHTTPURL_CONTENT,	5,  3000,		at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,STATE_QHTTPPOST,				5,  3000,		at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				STATE_REQUEST_TOKEN,		5,  3000,		at_httppost_action1, 				at_httppost_action2},
	{STATE_REQUEST_TOKEN,		STATE_READ_TOKEN,				5,  3000,		at_httptoken_action1, 			at_httptoken_action2},
	{STATE_READ_TOKEN,			STATE_SEND_DATA,				5,  3000,		at_readtoken_action1, 			at_readtoken_action2},
	{STATE_IDEL,     				STATE_IDEL,      				100,3000,		state_idel_action1, 				state_idel_action2 	},
};

nbiot_fsm_state_index_t* get_current_state_index()
{
	return &nbiot_fsm_state_index;
}

void init_at_statemachine()
{
	nbiot_fsm_state_index.cur_state = STATE_AT;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = &nbiot_state_list[0];
	current_state_index = 0;
}

void jump_to_next_at_statemachine() 
{
	++current_state_index;
	nbiot_fsm_state_index.cur_state = nbiot_state_list[current_state_index].cur_state;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = &nbiot_state_list[current_state_index];;
}

const nbiot_fsm_state_t* find_state_from_command(nbiot_state_e command)
{
	for (int i = 0; i < STATE_LENGTH; ++i) {
		if (nbiot_state_list[i].cur_state == command) {
			return &nbiot_state_list[i];
		}
	}
	return NULL;
}

ErrorStatus isCorrectCommand(const char* buffer, const char* command)
{
	if (strstr(buffer, command) != NULL) {
		return SUCCESS;
	} else {
		return ERROR;
	}
}

int at_reset_action1()
{
	reset_at_module();
	return 0;
}

action_result at_reset_action2(const char *command_buffer)
{
	return ACTION_SUCCESS;
}

int at_action1()
{
	printf("send command at\n");
	send_at_command("AT\r\n");
	return 0;
}

action_result at_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

int at_pin_action1(void)
{
	printf("send command AT+CPIN?\n");
	send_at_command("AT+CPIN?\r\n");
	return 0;
}

action_result at_pin_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "READY") == SUCCESS)  {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//creg register cs service
int at_creg_action1(void)
{
	printf("send command AT+CREG?\n");
	send_at_command("AT+CREG?\r\n");
	return 0;
}

action_result at_creg_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "0,1") == SUCCESS)  {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//cgreg register ps service
int at_cgreg_action1(void)
{
	printf("send command AT+CGREG?\n");
	send_at_command("AT+CGREG?\r\n");
	return 0;
}
action_result at_cgreg_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "0,1") == SUCCESS)  {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//  <contextID>, <context_type>, <apn>, <username>, <password>, <authentication>
int at_csgp_action1(void)
{
	printf("send command UNINET\n");
	send_at_command("AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1\r\n");
	return 0;
}

action_result at_csgp_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} if(isCorrectCommand(command_buffer, "AT+QICSGP=1") == SUCCESS) {
		return ACTION_WAIT_AGAIN;;
	} else {
		return ACTION_FAILED;
	}
}

//activate a pdp context
int at_act_action1(void)
{
	printf("send command AT+QIACT=1\n");
	send_at_command("AT+QIACT=1\r\n");
	return 0;
}

action_result at_act_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//query a pdp context
int at_query_act_action1(void)
{
	printf("send command AT+QIACT?\n");
	send_at_command("AT+QIACT?\r\n");
	return 0;
}
//<context_id>, <context_state: 0 deactivated, 1 activated>, <context_type 1 ipv4 2 ipv6, 3ipv4v6>, <ip_address>
action_result at_query_act_action2(const char *command_buffer)
{
	const char* find_position = strchr(command_buffer, '"');
	if (find_position == NULL) {
		return ACTION_FAILED;
	}
	const char* end_position = strchr(find_position+1, '"');
	if (end_position == NULL) {
		return ACTION_FAILED;
	}
	char ip_address[16];
	strncpy(ip_address, find_position+1, (end_position-find_position-1));
	set_ip_address(ip_address);
	return ACTION_SUCCESS;
}

//configure parameters for https server
int at_httpcfg_context_action1(void)
{
	send_at_command("AT+QHTTPCFG=\"contextid\",1\r\n");
	return 0;
}

action_result at_httpcfg_context_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}


//configure parameters for https header
int at_httpcfg_header_action1(void)
{
	send_at_command("AT+QHTTPCFG=\"requestheader\",1\r\n");
	return 0;
}

action_result at_httpcfg_header_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}


//configure url for https server
int at_httpurl_action1(void)
{
	send_at_command("AT+QHTTPURL=32,80\r\n");
	return 0;
}

action_result at_httpurl_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//configure the url address
int at_httpurl_add_action1(void)
{
	send_at_command("http://39.107.84.155/oauth/token");
	return 0;
}

action_result at_httpurl_add_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//post at command
int at_httppost_action1(void)
{
	send_at_command("AT+QHTTPPOST=266,80,80\r\n");
	return 0;
}

action_result at_httppost_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

int at_httptoken_action1(void)
{
	char* send_data = "POST /oauth/token HTTP/1.1\r\nHost: 39.107.84.155\r\nContent-Type: application/json\r\nContent-Length: 162\r\n\r\n{\"grant_type\":\"password\",\"username\":\"test\",\"password\":\"c70d12b4f791674b70b2\",\"client_id\":\"cb7fc9c8\",\"client_secret\":\"YyeEJfISMYMKZ6gsAyLkQwdhp6YEIQdCrPZohWz9OO4\"}";
	send_at_command(send_data);
	return 0;
}

action_result at_httptoken_action2(const char *command_buffer)
{	
	if (isCorrectCommand(command_buffer, "QHTTPPOST") == SUCCESS) {
		if (isCorrectCommand(command_buffer, "200") == SUCCESS) {
			return ACTION_SUCCESS;
		} else {
			return ACTION_FAILED;
		}
	} else if(isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_WAIT_AGAIN;;
	} else {
		return ACTION_FAILED;
	}
}

//read token command
int at_readtoken_action1(void)
{
	send_at_command("AT+QHTTPREAD=80\r\n");
	return 0;
}

action_result at_readtoken_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		get_token_from_buffer(command_buffer);
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}



int send_data_action1()
{
	return 0;
}

action_result send_data_action2(const char *command_buffer)
{
	return ACTION_SUCCESS;
}

int state_idel_action1()
{
	return 0;
}

action_result state_idel_action2(const char *command_buffer)
{
	return ACTION_SUCCESS;
}

/*
void send_at_command(char* command)
{
	HAL_StatusTypeDef result = HAL_OK;	
	int retry_count = 3;
	while(retry_count--) {
		result = HAL_UART_Transmit(&huart3, command, strlen(command), AT_TRANSMIT_TIME);
		if (result == HAL_OK) {
			return;
		}
	}
}
*/

void send_at_command(char *data)
{
 
  while(*data)
  {
    while(LL_USART_IsActiveFlag_TC(USART3)!=1);//�ȴ��������
    LL_USART_TransmitData8(USART3,(uint8_t)(*data & (uint8_t)0xff));//��������
    data++;
  }
  HAL_Delay(1000);
 
}
