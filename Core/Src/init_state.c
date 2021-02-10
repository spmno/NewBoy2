#include "utils.h"
#include "usart.h"

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
	} else if (isCorrectCommand(command_buffer, "CPIN") == SUCCESS) {
		return ACTION_WAIT_AGAIN;
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
	} else if(isCorrectCommand(command_buffer, "AT+QICSGP=1") == SUCCESS) {
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
	}  else if(isCorrectCommand(command_buffer, "QIACT") == SUCCESS) {
		printf("QIACT--\n");
		return ACTION_WAIT_AGAIN;;
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
		return ACTION_COMPLETED;
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
		return ACTION_SUCCESS;
	} else {
		return ACTION_FAILED;
	}
}

