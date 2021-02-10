#include "at_statemachine.h"
#include "usart.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "user_task.h"
#include "utils.h"
#include "data_saver.h"

//state 
#include "init_state.h"
#include "get_token_state.h"
#include "upload_state.h"
#include "version_check.h"
#include "download_version_state.h"

#define AT_TRANSMIT_TIME	200

const char* ip = "http://39.107.84.155";
const char* host = "39.107.84.155";
const char* app_name = "NewBoy2";
const float version = 1;

static nbiot_fsm_state_index_t nbiot_fsm_state_index;
static int current_state_index = 0;

static _Bool need_check_update = true;
static _Bool need_download_file = true;

static at_fsm_state_t *current_at_state_list;
static const at_fsm_state_t init_at_state_list[] = {
	{STATE_AT,       				3,  3000,	1000, at_action1,         			at_action2         		},
	{STATE_CPIN,     				3,  10000,	1000, at_pin_action1,       		at_pin_action2     		},
	{STATE_CREG,					3,  30000,	1000, at_creg_action1,  			at_creg_action2  		},
	{STATE_CGREG,					3,  30000,	1000, at_cgreg_action1, 	 		at_cgreg_action2  		},
	{STATE_QICSGP,					3,  1000,	1000, at_csgp_action1,  			at_csgp_action2  		},
	{STATE_QIACT,					3,  1000,	1000, at_act_action1,  				at_act_action2  		},
	{STATE_QIACT_RESULT,			3,  1000,	1000, at_query_act_action1, 		at_query_act_action2	},
	{STATE_QHTTPCFG_CONTEXT,		5,  3000,	1000, at_httpcfg_context_action1, 	at_httpcfg_context_action2},
};

static const at_fsm_state_t get_token_state_list[] = {
	{STATE_QHTTPREQ_HEADER,			5,  3000,	1000, at_httpcfg_header_action1, 	at_httpcfg_header_action2},
	{STATE_QHTTPURL,				5,  3000,	1000, at_httpurl_action1, 				at_httpurl_action2},
	{STATE_QHTTPURL_CONTENT,		5,  3000,	1000, at_httpurl_add_action1, 		at_httpurl_add_action2},
	{STATE_QHTTPPOST,				5,  3000,	1000, at_httppost_action1, 				at_httppost_action2},
	{STATE_REQUEST_TOKEN,			5,  3000,	1000, at_httptoken_action1, 			at_httptoken_action2},
	{STATE_READ_TOKEN,				5,  3000,	1000, at_readtoken_action1, 			at_readtoken_action2},
};

static const at_fsm_state_t upload_state_list[] = {
	{STATE_QHTTPREQ_HEADER,			5,  3000,	1000, at_httpcfg_header_action1, 	at_httpcfg_header_action2},
	{STATE_QHTTPURL1,				5,  3000,	1000, at_httpurl1_action1, 				at_httpurl1_action2},
	{STATE_QHTTPURL_CONTENT1,		5,  3000,	1000, at_httpurl1_add_action1, 		at_httpurl1_add_action2},
	{STATE_QHTTPPOST1,				5,  3000,	1000, at_httppost1_action1, 				at_httppost1_action2},
	{STATE_UPLOAD_INFO,				5,  3000,	1000, at_httpupload_action1, 			at_httpupload_action2},
	{STATE_READ_RESULT,				5,  3000,	3000, at_readresult_action1, 			at_readresult_action2},
};


static const at_fsm_state_t check_version_state_list[] = {
	{STATE_QHTTP_VERSION_URL,		5,  3000,	1000, at_httpurl_version_action1, 		at_httpurl_version_action2},
	{STATE_QHTTP_VERSION_CONTENT,	5,  3000,	1000, at_httpurl_version_add_action1, 	at_httpurl_version_add_action2},
	{STATE_QHTTP_VERSION_GET,		5,  3000,	1000, at_httpget_version_action1, 		at_httpget_version_action2},
	{STATE_READ_VERSION,			5,  3000,	1000, at_read_version_action1, 		at_read_version_action2},
};


static const at_fsm_state_t download_version_state_list[] = 
{
	{STATE_QHTTP_DOWNLOAD_URL,			5,  3000,	1000, at_httpurl_download_action1, 			at_httpurl_download_action2},
	{STATE_QHTTP_DOWNLOAD_CONTENT,		5,  3000,	1000, at_httpurl_content_download_action1, 	at_httpurl_content_download_action2},
	{STATE_QHTTP_DOWNLOAD_GET,			5,  3000,	1000, at_httpget_download_action1, 			at_httpget_download_action2},
	{STATE_READ_DOWNLOAD_URL,			5,  3000,	1000, at_read_download_url_action1, 		at_read_download_url_action2},
	{STATE_QHTTP_DOWNLOAD_FILE_URL,		5,  3000,	3000, at_httpurl_file_action1, 				at_httpurl_file_action2},
	{STATE_QHTTP_DOWNLOAD_FILE_CONTENT,	5,  3000,	1000, at_httpurl_content_file_action1, 		at_httpurl_content_file_action2},
	{STATE_QHTTP_DOWNLOAD_FILE_GET,		5,  3000,	1000, at_httpget_file_action1, 				at_httpget_file_action2},
	{STATE_QHTTP_DOWNLOAD_FILE_GET,		5,  3000,	1000, at_read_file_action1, 				at_read_file_action2},
};



nbiot_fsm_state_index_t* get_current_state_index()
{
	return &nbiot_fsm_state_index;
}


void init_at_statemachine(const at_fsm_state_t *task_list)
{
	current_at_state_list = (at_fsm_state_t *)task_list;
	
	nbiot_fsm_state_index.cur_state = STATE_AT;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = current_at_state_list;
	current_state_index = 0;
}

void jump_to_next_at_statemachine() 
{
	current_state_index++;
	nbiot_fsm_state_index.cur_state = current_at_state_list[current_state_index].cur_state;
	nbiot_fsm_state_index.init = 1;
	nbiot_fsm_state_index.trycnt = 0;
	nbiot_fsm_state_index.fsm_state = &current_at_state_list[current_state_index];
}

// switch task logic
void jump_to_next_at_task(void)
{
	// finish init
	if (current_at_state_list == init_at_state_list) {
		if (need_check_update) {
			init_at_statemachine(check_version_state_list);
		} else {
			init_at_statemachine(get_token_state_list);
		} 
	} else if (current_at_state_list == check_version_state_list) {
		if (need_download_file) {
			float version_oncloud = atof(get_version_string());
			if (version_oncloud > version) {
				init_at_statemachine(download_version_state_list);
				return;
			}
		}			
		init_at_statemachine(get_token_state_list);
	} else if (current_at_state_list == download_version_state_list) {
		save_update_flag();
	} else if (current_at_state_list == get_token_state_list) {
		init_at_statemachine(upload_state_list);
	} else if (current_at_state_list == upload_state_list) {
		init_at_statemachine(upload_state_list);
	} else {
		init_at_statemachine(init_at_state_list);
	}
}

void init_at_module(void)
{
	init_at_statemachine(init_at_state_list);
}


void jump_to_init_at_task(void)
{
	init_at_statemachine(init_at_state_list);
}


const at_fsm_state_t* find_state_from_command(nbiot_state_e command)
{
	for (int i = 0; i < STATE_LENGTH; ++i) {
		if (current_at_state_list[i].cur_state == command) {
			return &current_at_state_list[i];
		}
	}
	return NULL;
}


