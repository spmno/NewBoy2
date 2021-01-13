#include "at_define.h"

//at command
int at_action1(void);
action_result at_action2(const char *command_buffer);
//cpin command sim card query
int at_pin_action1(void);
action_result at_pin_action2(const char *command_buffer);
//creg register cs service
int at_creg_action1(void);
action_result at_creg_action2(const char *command_buffer);
//cgreg register ps service
int at_cgreg_action1(void);
action_result at_cgreg_action2(const char *command_buffer);
//configure parameters of a tcp/ip context
int at_csgp_action1(void);
action_result at_csgp_action2(const char *command_buffer);
//activate a pdp context
int at_act_action1(void);
action_result at_act_action2(const char *command_buffer);
//query a pdp context
int at_query_act_action1(void);
action_result at_query_act_action2(const char *command_buffer);
//configure parameters for https server
int at_httpcfg_context_action1(void);
action_result at_httpcfg_context_action2(const char *command_buffer);

//configure parameters for https header
int at_httpcfg_header_action1(void);
action_result at_httpcfg_header_action2(const char *command_buffer);

