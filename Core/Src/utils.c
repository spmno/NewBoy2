#include "utils.h"
#include "stdio.h"
<<<<<<< HEAD
#include "cJSON.h"
=======
>>>>>>> 48bc69e74505f74fba0efc80fd7d49c4c59cc101

#define AT_COMMAND_BUFFER_LEN 64
static char current_at_command_buffer[AT_COMMAND_BUFFER_LEN];

#define IP_ADDRESS_BUFFER_LEN 16
static char ip_address[IP_ADDRESS_BUFFER_LEN];

<<<<<<< HEAD
#define TOKEN_LEN	128
static char access_token[TOKEN_LEN];
static char refresh_token[TOKEN_LEN];

const char* get_at_command_from_buffer(const char* buffer) 
{
#if 0
=======
const char* get_at_command_from_buffer(const char* buffer) 
{
>>>>>>> 48bc69e74505f74fba0efc80fd7d49c4c59cc101
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
<<<<<<< HEAD
#else
	strcpy(current_at_command_buffer, buffer);
#endif
=======
	
>>>>>>> 48bc69e74505f74fba0efc80fd7d49c4c59cc101
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

<<<<<<< HEAD
_Bool get_token_from_buffer(const char* buffer)
{
	printf("get_token_from_buffer\n");
	char *token_str = "\"access_token\"";
	const char* token_position = strstr(buffer, token_str);
	if (NULL == token_position) {
		printf("not found { \n");
		return false;
	}
	const char* token_end_posiont = token_position+strlen(token_str)+1;
	const char* start_dqmark_position = strchr(token_end_posiont, '"') + 1;
	const char* end_dqmark_position = strchr(start_dqmark_position+1, '"');
	strncpy(access_token, start_dqmark_position, end_dqmark_position-start_dqmark_position);
	printf("access_token:%s\n", access_token);
	return true;
}
/*
_Bool get_token_from_buffer(const char* buffer)
{
	printf("get_token_from_buffer\n");
	const char* left_brace_position = strstr(buffer, "{");
	if (NULL == left_brace_position) {
		printf("not found { \n");
		return false;
	}
	const char* right_brace_position = strstr(buffer, "}");
		if (NULL == left_brace_position) {
		printf("not found } \n");
		return false;
	}
	
	char *token_json_buffer = (char*)malloc(256);
	if(NULL == token_json_buffer) {
		printf("malloc error!");
	}
	strncpy(token_json_buffer, left_brace_position, right_brace_position-left_brace_position+1);
	printf("token:%s\n", token_json_buffer);
	cJSON* root = cJSON_Parse(token_json_buffer);
	if (!root) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	
	cJSON* access_token_item = cJSON_GetObjectItem(root, "access_token");
	if (!access_token_item) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	
	cJSON* refresh_token_item = cJSON_GetObjectItem(root, "refresh_token");
	if (!refresh_token_item) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	
	strcpy(access_token, access_token_item->valuestring);
	strcpy(refresh_token, refresh_token_item->valuestring);
	printf("access_token:%s, refresh_token:%s\n", access_token, refresh_token);
	cJSON_Delete(root);
	free(token_json_buffer);
	return true;
}
*/
_Bool get_result_from_buffer(const char* buffer)
{
	printf("get_result_from_buffer\n");
	return true;
}

char* get_access_token(void)
{
	return access_token;
}


=======
>>>>>>> 48bc69e74505f74fba0efc80fd7d49c4c59cc101
