#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

const char* get_at_command_from_buffer(const char* buffer);

const char* get_ip_address(void);

void set_ip_address(const char* ip_address_para);

_Bool get_token_from_buffer(const char* buffer);

_Bool get_result_from_buffer(const char* buffer);

char* get_access_token(void);

