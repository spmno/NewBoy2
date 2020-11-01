#include "utils.h"
#include "stdio.h"

#define AT_COMMAND_BUFFER_LEN 64
static char current_at_command_buffer[AT_COMMAND_BUFFER_LEN];

#define IP_ADDRESS_BUFFER_LEN 16
static char ip_address[IP_ADDRESS_BUFFER_LEN];

const char* get_at_command_from_buffer(const char* buffer) 
{
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

