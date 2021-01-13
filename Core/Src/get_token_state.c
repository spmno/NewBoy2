#include "get_token_state.h"
#include "utils.h"
#include "usart.h"

static char token_command[300];
static char token_request_body[128];

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

