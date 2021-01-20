#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "at_define.h"

extern const char* ip;
extern const char* host;
extern const char* app_name;

const char* get_at_command_from_buffer(const char* buffer);

const char* get_ip_address(void);

void set_ip_address(const char* ip_address_para);

_Bool get_token_from_buffer(const char* buffer);

_Bool get_result_from_buffer(const char* buffer);

_Bool get_version_from_buffer(const char* buffer);

_Bool get_file_url_form_buffer(const char* buffer);

char* get_access_token(void);

ErrorStatus isCorrectCommand(const char* buffer, const char* command);



