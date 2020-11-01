#include "at_statemachine.h"
#include "usart.h"
#include "stdlib.h"
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

int set_param_action1(void);
action_result set_param_action2(const char *command_buffer);
int send_data_action1(void);
action_result send_data_action2(const char *command_buffer);
int state_idel_action1(void);

action_result state_idel_action2(const char *command_buffer);
void send_at_command(char* command);

static nbiot_fsm_state_index_t nbiot_fsm_state_index;
static int current_state_index = 0;

//当前状态, 下一个状态, 重试次数, 等待时间, 发送, 接收
static const nbiot_fsm_state_t nbiot_state_list[] =
{
	{STATE_HAL_RESET,				STATE_AT, 	         		1,  300, 		at_reset_action1, 					at_reset_action2		},
	{STATE_AT,       				STATE_CPIN,				 			3,  3000,		at_action1,         				at_action2         	},
	{STATE_CPIN,     				STATE_CREG,	 						3,  10000,	at_pin_action1,       			at_pin_action2     	},
	{STATE_CREG,						STATE_CGREG,		 				3,  30000,	at_creg_action1,  					at_creg_action2  		},
	{STATE_CGREG,						STATE_QICSGP,		 				3,  30000,	at_cgreg_action1, 	 				at_cgreg_action2  	},
	{STATE_QICSGP,					STATE_QIACT,		 				3,  1000,		at_csgp_action1,  					at_csgp_action2  		},
	{STATE_QIACT,						STATE_QIACT_RESULT,		 	3,  1000,		at_act_action1,  						at_act_action2  		},
	{STATE_QIACT,						STATE_QIACT_RESULT,		 	3,  1000,		at_act_action1,  						at_act_action2  		},
	{STATE_QIACT_RESULT,		STATE_QHTTPCFG_CONTEXT,	3,  1000,		at_query_act_action1, 			at_query_act_action2},
	{STATE_QHTTPCFG_CONTEXT,STATE_QHTTPREQ_HEADER,	5,  3000,		at_httpcfg_context_action1, at_httpcfg_context_action2},
	{STATE_QHTTPREQ_HEADER,	STATE_QHTTPURL,					5,  3000,		at_httpcfg_header_action1, 	at_httpcfg_header_action2},
	{STATE_QHTTPURL,				STATE_QHTTPURL_CONTENT,	5,  3000,		at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,STATE_QHTTPPOST,				5,  3000,		at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				STATE_QHTTPURL_CONTENT,	5,  3000,		at_httppost_action1, 				at_httppost_action2},
	{STATE_QHTTPPOST,				STATE_QHTTPURL_CONTENT,	5,  3000,		at_httppost_action1, 				at_httppost_action2},
	{STATE_IDEL,     				STATE_IDEL,      				100,3000,		state_idel_action1, 				state_idel_action2 	},
};

nbiot_fsm_state_index_t* get_current_state_index()
{
	return &nbiot_fsm_state_index;
}

void init_at_statemachine()
{
	nbiot_fsm_state_index.cur_state = STATE_HAL_RESET;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = &nbiot_state_list[current_state_index];
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
	osThreadFlagsSet(gps_task_handle, AT_RESET_FLAG);
	return 0;
}

action_result at_reset_action2(const char *command_buffer)
{
	return ACTION_SUCCESS;
}

int at_action1()
{
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
	send_at_command("AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1\r\n");
	return 0;
}

action_result at_csgp_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//activate a pdp context
int at_act_action1(void)
{
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

int set_param_action1()
{
	return 0;
}

action_result set_param_action2(const char *command_buffer)
{
	return ACTION_SUCCESS;
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

void send_at_command(char* command)
{
	HAL_StatusTypeDef result = HAL_OK;	
	int retry_count = 3;
	while(retry_count--) {
		result = HAL_UART_Transmit(&huart3, (uint8_t*)command, strlen(command), AT_TRANSMIT_TIME);
		if (result == HAL_OK) {
			return;
		}
	}
}


