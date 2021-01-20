#include "download_version_state.h"
#include "data_saver.h"
#include "usart.h"

extern char url_content[64];

//configure url for download
int at_httpurl_download_action1(void)
{
	printf("send command AT+QHTTPURLDOWNLOAD\n");
	sprintf(url_content, "%s/api/v1/app_versions/url?name=%s&version=%s", ip, app_name, get_version_string());
	int url_length = strlen(url_content);
	char url_command[32];
	sprintf(url_command, "AT+QHTTPURL=%d,80\r\n", url_length);
	send_at_command(url_command);
	return 0;
}

action_result at_httpurl_download_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//configure the download url address
int at_httpurl_content_download_action1(void)
{
	printf("send download content\n");
	send_at_command(url_content);
	return 0;
}

action_result at_httpurl_content_download_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}


int at_httpget_download_action1(void)
{
	printf("send get data\n");
	send_at_command("AT+QHTTPGET=80\r\n");
	return 0;
}

action_result at_httpget_download_action2(const char *command_buffer)
{	
	if (isCorrectCommand(command_buffer, "200") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//read download url command
int at_read_download_url_action1(void)
{
	printf("send version reader\n");
	HAL_Delay(100);
	send_at_command("AT+QHTTPREAD=80\r\n");
	return 0;
}

action_result at_read_download_url_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		if(get_file_url_form_buffer(command_buffer)) {
			return ACTION_SUCCESS;
		}
	}
	return ACTION_FAILED;
}

//configure url for download
int at_httpurl_file_action1(void)
{
	printf("send command AT+QHTTPURLBIN\n");
	sprintf(url_content, "%s/uploads/app_version/1/NewBoy2.hex", ip);
	int url_length = strlen(url_content);
	char url_command[32];
	sprintf(url_command, "AT+QHTTPURL=%d,80\r\n", url_length);
	send_at_command(url_command);
	return 0;
}

action_result at_httpurl_file_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//configure the download url address
int at_httpurl_content_file_action1(void)
{
	printf("send file content\n");
	send_at_command(url_content);
	return 0;
}

action_result at_httpurl_content_file_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

int at_httpget_file_action1(void)
{
	printf("send get data\n");
	send_at_command("AT+QHTTPGET=80\r\n");
	return 0;
}

action_result at_httpget_file_action2(const char *command_buffer)
{	
	if (isCorrectCommand(command_buffer, "200") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//read download url command
int at_read_file_action1(void)
{
	printf("send version reader\n");
	HAL_Delay(100);
	send_at_command("AT+QHTTPREADFILE=\"UFS:NewBoy2.hex\", 80\r\n");
	return 0;
}

action_result at_read_file_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		printf("read file again\n");
		return ACTION_WAIT_AGAIN;;
	} else if (isCorrectCommand(command_buffer, "QHTTPREADFILE") == SUCCESS) {
		printf("read file again\n");
		return ACTION_COMPLETED;;
	}
	return ACTION_FAILED;
}

