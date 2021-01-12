#include "at_statemachine.h"
#include "usart.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "user_task.h"
#include "utils.h"
#include "data_saver.h"

#define AT_TRANSMIT_TIME	200

static const char* ip = "http://39.107.84.155";
static const char* host = "39.107.84.155";
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

//configure url for update
int at_httpurl1_action1(void);
action_result at_httpurl1_action2(const char *command_buffer);

//configure the update url address
int at_httpurl1_add_action1(void);
action_result at_httpurl1_add_action2(const char *command_buffer);

//post bike data command
int at_httppost1_action1(void);
action_result at_httppost1_action2(const char *command_buffer);

int at_httpupload_action1(void);
action_result at_httpupload_action2(const char *command_buffer);
		
int at_readresult_action1(void);
action_result at_readresult_action2(const char *command_buffer);


int send_data_action1(void);
action_result send_data_action2(const char *command_buffer);

int state_idel_action1(void);
action_result state_idel_action2(const char *command_buffer);

void send_at_command(char* command);

static nbiot_fsm_state_index_t nbiot_fsm_state_index;
static int current_state_index = 0;

static _Bool need_check_update = true;
static _Bool need_download_file = false;

/*
static const nbiot_fsm_state_t nbiot_state_list[] =
{
	//{STATE_HAL_RESET,				STATE_AT, 	         		1,  3000, 	at_reset_action1, 					at_reset_action2		},
	{STATE_AT,       				STATE_CPIN,				 	3,  3000,	1000, at_action1,         				at_action2         	},
	{STATE_CPIN,     				STATE_CREG,	 				3,  10000,	1000, at_pin_action1,       			at_pin_action2     	},
	{STATE_CREG,					STATE_CGREG,		 		3,  30000,	1000, at_creg_action1,  					at_creg_action2  		},
	{STATE_CGREG,					STATE_QICSGP,		 		3,  30000,	1000, at_cgreg_action1, 	 				at_cgreg_action2  	},
	{STATE_QICSGP,					STATE_QIACT,		 		3,  1000,	1000, at_csgp_action1,  					at_csgp_action2  		},
	{STATE_QIACT,					STATE_QIACT_RESULT,		 	3,  1000,	1000, at_act_action1,  						at_act_action2  		},
	{STATE_QIACT_RESULT,			STATE_QHTTPCFG_CONTEXT,		3,  1000,	1000, at_query_act_action1, 			at_query_act_action2},
	{STATE_QHTTPCFG_CONTEXT,		STATE_QHTTPREQ_HEADER,		5,  3000,	1000, at_httpcfg_context_action1, at_httpcfg_context_action2},
	{STATE_QHTTPREQ_HEADER,			STATE_QHTTPURL,				5,  3000,	1000, at_httpcfg_header_action1, 	at_httpcfg_header_action2},
	{STATE_QHTTPURL,				STATE_QHTTPURL_CONTENT,		5,  3000,	1000, at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,		STATE_QHTTPPOST,			5,  3000,	1000, at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				STATE_REQUEST_TOKEN,		5,  3000,	1000, at_httppost_action1, 				at_httppost_action2},
	{STATE_REQUEST_TOKEN,			STATE_READ_TOKEN,			5,  3000,	1000, at_httptoken_action1, 			at_httptoken_action2},
	{STATE_READ_TOKEN,				STATE_QHTTPURL1,			5,  3000,	1000, at_readtoken_action1, 			at_readtoken_action2},
	{STATE_QHTTPURL1,				STATE_QHTTPURL_CONTENT1,	5,  3000,	1000, at_httpurl1_action1, 				at_httpurl1_action2},
	{STATE_QHTTPURL_CONTENT1,		STATE_QHTTPPOST1,			5,  3000,	1000, at_httpurl1_add_action1, 		at_httpurl1_add_action2},
	{STATE_QHTTPPOST1,				STATE_UPLOAD_INFO,			5,  3000,	1000, at_httppost1_action1, 				at_httppost1_action2},
	{STATE_UPLOAD_INFO,				STATE_READ_RESULT,			5,  3000,	1000, at_httpupload_action1, 			at_httpupload_action2},
	{STATE_READ_RESULT,				STATE_QHTTPPOST1,			5,  3000,	3000, at_readresult_action1, 			at_readresult_action2},

};
*/

static at_fsm_state_t *current_at_state_list;
static const at_fsm_state_t init_at_state_list[] = 
{
	{STATE_AT,       				3,  3000,	1000, at_action1,         			at_action2         		},
	{STATE_CPIN,     				3,  10000,	1000, at_pin_action1,       		at_pin_action2     		},
	{STATE_CREG,					3,  30000,	1000, at_creg_action1,  			at_creg_action2  		},
	{STATE_CGREG,					3,  30000,	1000, at_cgreg_action1, 	 		at_cgreg_action2  		},
	{STATE_QICSGP,					3,  1000,	1000, at_csgp_action1,  			at_csgp_action2  		},
	{STATE_QIACT,					3,  1000,	1000, at_act_action1,  				at_act_action2  		},
	{STATE_QIACT_RESULT,			3,  1000,	1000, at_query_act_action1, 		at_query_act_action2	},
	{STATE_QHTTPCFG_CONTEXT,		5,  3000,	1000, at_httpcfg_context_action1, 	at_httpcfg_context_action2},
	{STATE_QHTTPREQ_HEADER,			5,  3000,	1000, at_httpcfg_header_action1, 	at_httpcfg_header_action2},
};

static const at_fsm_state_t get_token_state_list[] = 
{
	{STATE_QHTTPURL,				5,  3000,	1000, at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,		5,  3000,	1000, at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				5,  3000,	1000, at_httppost_action1, 				at_httppost_action2},
	{STATE_REQUEST_TOKEN,			5,  3000,	1000, at_httptoken_action1, 			at_httptoken_action2},
	{STATE_READ_TOKEN,				5,  3000,	1000, at_readtoken_action1, 			at_readtoken_action2},
};

static const at_fsm_state_t upload_state_list[] = 
{
	{STATE_QHTTPURL1,				5,  3000,	1000, at_httpurl1_action1, 				at_httpurl1_action2},
	{STATE_QHTTPURL_CONTENT1,		5,  3000,	1000, at_httpurl1_add_action1, 		at_httpurl1_add_action2},
	{STATE_QHTTPPOST1,				5,  3000,	1000, at_httppost1_action1, 				at_httppost1_action2},
	{STATE_UPLOAD_INFO,				5,  3000,	1000, at_httpupload_action1, 			at_httpupload_action2},
	{STATE_READ_RESULT,				5,  3000,	3000, at_readresult_action1, 			at_readresult_action2},
};

static const at_fsm_state_t check_version_state_list[] = 
{
	{STATE_QHTTPURL,				5,  3000,	1000, at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,		5,  3000,	1000, at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				5,  3000,	1000, at_httppost_action1, 				at_httppost_action2},
	{STATE_REQUEST_TOKEN,			5,  3000,	1000, at_httptoken_action1, 			at_httptoken_action2},
	{STATE_READ_TOKEN,				5,  3000,	1000, at_readtoken_action1, 			at_readtoken_action2},
	{STATE_QHTTPURL1,				5,  3000,	1000, at_httpurl1_action1, 				at_httpurl1_action2},
	{STATE_QHTTPURL_CONTENT1,		5,  3000,	1000, at_httpurl1_add_action1, 		at_httpurl1_add_action2},
	{STATE_QHTTPPOST1,				5,  3000,	1000, at_httppost1_action1, 				at_httppost1_action2},
	{STATE_UPLOAD_INFO,				5,  3000,	1000, at_httpupload_action1, 			at_httpupload_action2},
	{STATE_READ_RESULT,				5,  3000,	3000, at_readresult_action1, 			at_readresult_action2},
};

static const at_fsm_state_t download_version_state_list[] = 
{
	{STATE_QHTTPURL,				5,  3000,	1000, at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,		5,  3000,	1000, at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				5,  3000,	1000, at_httppost_action1, 				at_httppost_action2},
	{STATE_REQUEST_TOKEN,			5,  3000,	1000, at_httptoken_action1, 			at_httptoken_action2},
	{STATE_READ_TOKEN,				5,  3000,	1000, at_readtoken_action1, 			at_readtoken_action2},
	{STATE_QHTTPURL1,				5,  3000,	1000, at_httpurl1_action1, 				at_httpurl1_action2},
	{STATE_QHTTPURL_CONTENT1,		5,  3000,	1000, at_httpurl1_add_action1, 		at_httpurl1_add_action2},
	{STATE_QHTTPPOST1,				5,  3000,	1000, at_httppost1_action1, 				at_httppost1_action2},
	{STATE_UPLOAD_INFO,				5,  3000,	1000, at_httpupload_action1, 			at_httpupload_action2},
	{STATE_READ_RESULT,				5,  3000,	3000, at_readresult_action1, 			at_readresult_action2},
};

static char token_command[300];
static char token_request_body[128];

nbiot_fsm_state_index_t* get_current_state_index()
{
	return &nbiot_fsm_state_index;
}


void init_at_statemachine(const at_fsm_state_t *task_list)
{
	current_at_state_list = (at_fsm_state_t *)task_list;
	
	nbiot_fsm_state_index.cur_state = STATE_AT;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = current_at_state_list;
	current_state_index = 0;
}

void jump_to_next_at_statemachine() 
{
	current_state_index++;
	nbiot_fsm_state_index.cur_state = current_at_state_list[current_state_index].cur_state;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = &current_at_state_list[current_state_index];
}

// switch task logic
void jump_to_next_at_task(void)
{
	// finish init
	if (current_at_state_list == init_at_state_list) {
		if (need_check_update) {
			init_at_statemachine(check_version_state_list);
		} else {
			init_at_statemachine(get_token_state_list);
		} 
	} else if (current_at_state_list == check_version_state_list) {
		if (need_download_file) {
			init_at_statemachine(download_version_state_list);
		} else {
			init_at_statemachine(get_token_state_list);
		}
	} else if (current_at_state_list == download_version_state_list) {
		
	} else if (current_at_state_list == get_token_state_list) {
		init_at_statemachine(upload_state_list);
	} else if (current_at_state_list == upload_state_list) {
		init_at_statemachine(upload_state_list);
	} else {
		init_at_statemachine(init_at_state_list);
	}
}

void init_at_module(void)
{
	init_at_statemachine(init_at_state_list);
}


void jump_to_init_at_task(void)
{
	init_at_statemachine(init_at_state_list);
}

void push_task(nbiot_fsm_state_t* task_attr)
{
	//task_stack[++current_task_index] = task_attr;
}

const at_fsm_state_t* find_state_from_command(nbiot_state_e command)
{
	for (int i = 0; i < STATE_LENGTH; ++i) {
		if (current_at_state_list[i].cur_state == command) {
			return &current_at_state_list[i];
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
		printf("at_csgp_action2 s\n");
		return ACTION_SUCCESS;
	} if(isCorrectCommand(command_buffer, "AT+QICSGP=1") == SUCCESS) {
		printf("at_csgp_action2 a\n");
		return ACTION_WAIT_AGAIN;;
	} else {
		printf("at_csgp_action2 f\n");
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
	printf("send command AT+QHTTPCFG context\n");
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
	printf("send command AT+QHTTPCFG header\n");
	send_at_command("AT+QHTTPCFG=\"requestheader\",1\r\n");
	return 0;
}

action_result at_httpcfg_header_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_COMPLETED;
	} else {
		return ACTION_FAILED;
	}
}


//configure url for https server
int at_httpurl_action1(void)
{
	printf("send command AT+QHTTPURL\n");
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
	printf("send url content\n");
	char token_url[48];
	sprintf(token_url, "%s/oauth/token", ip);
	send_at_command(token_url);
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
static char* username = "test";
static char* password = "c70d12b4f791674b70b2";
static char* client_id = "cb7fc9c8";
static char* client_secret = "YyeEJfISMYMKZ6gsAyLkQwdhp6YEIQdCrPZohWz9OO4";
static char* bike_id = "20201027001";

//post at command
int at_httppost_action1(void)
{
	printf("send command AT+QHTTPPOST\n");
	sprintf(token_request_body, "{\"grant_type\":\"password\",\"username\":\"%s\",\"password\":\"%s\",\"client_id\":\"%s\",\"client_secret\":\"%s\"}", 
		username, password, client_id, client_secret);
	int request_body_length = strlen(token_request_body);
	sprintf(token_command, "POST /oauth/token HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s", 
		host, request_body_length, token_request_body);
	
	char post_config_command[48];
	int command_length = strlen(token_command);
	sprintf(post_config_command, "AT+QHTTPPOST=%d,80,80\r\n", command_length); 
	send_at_command(post_config_command);
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
	printf("send post data\n");
	send_at_command(token_command);
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
	printf("send command http reader\n");
	HAL_Delay(100);
	send_at_command("AT+QHTTPREAD=80\r\n");
	return 0;
}

action_result at_readtoken_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		if(get_token_from_buffer(command_buffer)) {
			return ACTION_COMPLETED;
		}
	}
	
	return ACTION_FAILED;
}

//configure url for update
int at_httpurl1_action1(void)
{
	printf("send command AT+QHTTPURL1\n");
	send_at_command("AT+QHTTPURL=52,80\r\n");
	return 0;
}

action_result at_httpurl1_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//configure the update url address
int at_httpurl1_add_action1(void)
{
	printf("send url1 content\n");
	char url_content[64];
	sprintf(url_content, "%s/api/v1/bikes/upload/%s", ip, bike_id);
	send_at_command(url_content);
	return 0;
}

action_result at_httpurl1_add_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//post bike data command
static char send_data[512];
static char send_content[128];
int at_httppost1_action1(void)
{
	char post_buffer[32];
	int post_len = 0;
	printf("send command AT+QHTTPPOST1\n");
	gps_info *gps_info_pointer = get_gps_info();
	sprintf(send_content, "{\"longitude\":\"%0.4f\",\"latitude\":\"%0.4f\",\"diag_info\":{}", gps_info_pointer->longitude, gps_info_pointer->latitude);
	int send_content_length = strlen(send_content);
	sprintf(send_data, "POST /api/v1/bikes/upload/%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\nAuthorization: Bearer %s\r\n\r\n%s}", 
			bike_id,
			host,
			send_content_length,
			get_access_token(), 
			send_content
			);
	post_len = strlen(send_data);
	sprintf(post_buffer, "AT+QHTTPPOST=%d,80,80\r\n", post_len);
	send_at_command(post_buffer);
	printf("len=%d\n", post_len);
	return 0;
}

action_result at_httppost1_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}


int at_httpupload_action1(void)
{
	printf("send upload %s\n", send_data);
	HAL_Delay(100);
	send_at_command(send_data);
	return 0;
}

action_result at_httpupload_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "QHTTPPOST") == SUCCESS) {
		if (isCorrectCommand(command_buffer, "201") == SUCCESS) {
			printf("action2 success\n");
			return ACTION_SUCCESS;
		} else {
			printf("action2 failed\n");
			return ACTION_FAILED;
		}
	} else if(isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		printf("action2 again\n");
		return ACTION_WAIT_AGAIN;;
	} else {
		printf("action2 failed\n");
		return ACTION_FAILED;
	}
}


int at_readresult_action1()
{
	printf("send command result reader\n");
	send_at_command("AT+QHTTPREAD=80\r\n");
	return 0;
}

action_result at_readresult_action2(const char *command_buffer)
{	
	if ((isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) || isCorrectCommand(command_buffer, "QHTTPREAD") == SUCCESS) {
		get_result_from_buffer(command_buffer);
		return ACTION_COMPLETED;
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
	printf("upload date.\n");
	
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
    while(LL_USART_IsActiveFlag_TC(USART3)!=1);//
    LL_USART_TransmitData8(USART3,(uint8_t)(*data & (uint8_t)0xff));//
    data++;
  }
}

