#include "utils.h"
#include "stdio.h"
#include "cJSON.h"

#define AT_COMMAND_BUFFER_LEN 64
static char current_at_command_buffer[AT_COMMAND_BUFFER_LEN];

#define IP_ADDRESS_BUFFER_LEN 16
static char ip_address[IP_ADDRESS_BUFFER_LEN];

#define TOKEN_LEN	128
static char access_token[TOKEN_LEN];
static char refresh_token[TOKEN_LEN];

const char* get_at_command_from_buffer(const char* buffer) 
{
#if 0
	const char* at_command_pos = strstr(buffer, "\r\n");
	if (NULL == at_command_pos) {
		return NULL;
	}
	at_command_pos += 2;
	const char* at_command_end_pos = strstr(at_command_end_pos, "\r\n");
	if (NULL == at_command_end_pos) {
		return NULL;
	}
	strncpy(current_at_command_buffer, at_command_pos, (at_command_end_pos-at_command_pos));
#else
	strcpy(current_at_command_buffer, buffer);
#endif
	return current_at_command_buffer;
}

inline const char* get_ip_address()
{
	return ip_address;
}

void set_ip_address(const char* ip_address_para)
{
	strcpy(ip_address, ip_address_para);
	printf("set ip: %s\n", ip_address);
}


_Bool get_token_from_buffer(const char* buffer)
{
	const char* left_brace_position = strstr(buffer, "{");
	if (NULL == left_brace_position) {
		return false;
	}
	const char* right_brace_position = strstr(buffer, "}");
		if (NULL == left_brace_position) {
		return false;
	}
	
	char token_json_buffer[256];
	strncpy(token_json_buffer, left_brace_position, right_brace_position-left_brace_position+1);
	cJSON* root = cJSON_Parse(token_json_buffer);
	if (!root) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	
	cJSON* access_token_item = cJSON_GetObjectItem(root, "access_token");
	if (!access_token_item) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	
	strcpy(access_token, access_token_item->string);
	return true;
}

