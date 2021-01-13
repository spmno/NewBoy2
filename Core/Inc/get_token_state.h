#include "at_define.h"

//configure url for https server
int at_httpurl_action1(void);
action_result at_httpurl_action2(const char *command_buffer);

//configure the url address
int at_httpurl_add_action1(void);
action_result at_httpurl_add_action2(const char *command_buffer);

//post at command
int at_httppost_action1(void);
action_result at_httppost_action2(const char *command_buffer);

//send token request
int at_httptoken_action1(void);
action_result at_httptoken_action2(const char *command_buffer);

//read token command
int at_readtoken_action1(void);
action_result at_readtoken_action2(const char *command_buffer);

