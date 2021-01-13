#include "upload_state.h"
#include "usart.h"
#include "data_saver.h"

static char* bike_id = "20201027001";

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


