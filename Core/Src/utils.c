#include "utils.h"
#include "stdio.h"
#include "data_saver.h"

#define IP_ADDRESS_BUFFER_LEN 16
static char ip_address[IP_ADDRESS_BUFFER_LEN];

#define TOKEN_LEN	128
static char access_token[TOKEN_LEN];
static char refresh_token[TOKEN_LEN];

static char current_at_command_buffer[256];
extern char version_string[16];
extern char download_file_url[128];

ErrorStatus isCorrectCommand(const char* buffer, const char* command)
{
	if (strstr(buffer, command) != NULL) {
		return SUCCESS;
	} else {
		return ERROR;
	}
}

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

_Bool get_version_from_buffer(const char* buffer)
{
	printf("get_version_buffer\n");
	char *version_str = "\"version\"";
	const char* version_position = strstr(buffer, version_str);
	if (NULL == version_position) {
		printf("not found { \n");
		return false;
	}
	const char* token_end_posiont = version_position+strlen(version_str)+1;
	const char* start_dqmark_position = strchr(token_end_posiont, '"') + 1;
	const char* end_dqmark_position = strchr(start_dqmark_position+1, '"');
	strncpy(version_string, start_dqmark_position, end_dqmark_position-start_dqmark_position);
	printf("version:%s\n", version_string);
	return true;
}

_Bool get_file_url_form_buffer(const char* buffer)
{
	printf("get_file_url_buffer\n");
	char *version_str = "\"url\"";
	const char* version_position = strstr(buffer, version_str);
	if (NULL == version_position) {
		printf("not found { \n");
		return false;
	}
	const char* token_end_posiont = version_position+strlen(version_str)+1;
	const char* start_dqmark_position = strchr(token_end_posiont, '"') + 1;
	const char* end_dqmark_position = strchr(start_dqmark_position+1, '"');
	strncpy(download_file_url, start_dqmark_position, end_dqmark_position-start_dqmark_position);
	printf("fileurl:%s\n", download_file_url);
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


