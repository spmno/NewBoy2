#include "version_check.h"
#include "usart.h"

extern char url_content[80];

int at_httpurl_version_action1(void)
{
	printf("send command AT+QHTTPURL_VERSION\n");
	sprintf(url_content, "%s/api/v1/app_versions/newest?name=%s", ip, app_name);
	int url_length = strlen(url_content);
	char url_command[32];
	sprintf(url_command, "AT+QHTTPURL=%d,80\r\n", url_length);
	send_at_command(url_command);
	return 0;
}

action_result at_httpurl_version_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//configure the update url address
int at_httpurl_version_add_action1(void)
{
	printf("url=%s\n", url_content);
	send_at_command(url_content);
	return 0;
}

action_result at_httpurl_version_add_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "OK") == SUCCESS) {
		return ACTION_SUCCESS; 
	} else {
		return ACTION_FAILED;
	}
}

int at_httpget_version_action1(void)
{
	printf("send get v\n");
	HAL_Delay(1000);
	send_at_command("AT+QHTTPGET=80\r\n");
	return 0;
}

action_result at_httpget_version_action2(const char *command_buffer)
{	
	if (isCorrectCommand(command_buffer, "200") == SUCCESS) {
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

//read version command
int at_read_version_action1(void)
{
	printf("send version reader\n");
	HAL_Delay(100);
	send_at_command("AT+QHTTPREAD=80\r\n");
	return 0;
}

action_result at_read_version_action2(const char *command_buffer)
{
	if (isCorrectCommand(command_buffer, "CONNECT") == SUCCESS) {
		if(get_version_from_buffer(command_buffer)) {
			return ACTION_COMPLETED;
		}
	}
	return ACTION_FAILED;
}

