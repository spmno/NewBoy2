#ifndef _AT_DEFINE_H_
#define _AT_DEFINE_H_

#include "stm32g4xx_hal.h"

typedef enum
{
  //STATE_HAL_RESET= 0x01,  			/*reset module*/
	STATE_AT,               			/*send at command wait for response ok*/
	STATE_CPIN,             			/*query SIM card status??if failed to identify sim card in 20s, then reboot the module*/
	STATE_CREG,							/* query CS service, if failed to register on CS domain service in 90s, then reboot the module */
	STATE_CGREG,						/* query PS service, if failed to register on PS domain service in 60s, then reboot the module */
	STATE_QICSGP,						/* configure parameters of a tcp/ip context */
	STATE_QIACT,						/* activate a pdp context */
	STATE_QIACT_RESULT,     			/* query pdp context */
	STATE_QHTTPCFG_CONTEXT,				/* configure parameter for http server */
	STATE_QHTTPREQ_HEADER,				/* configure request header */
	//STATE_QHTTPRES_HEADER,			/* configure response header */
	STATE_QHTTPURL,						/* configure url for http server */
	STATE_QHTTPURL_CONTENT,				/* configure url add for http server */
	STATE_QHTTPPOST,					/* send post request */
	STATE_REQUEST_TOKEN,    			/* send post for token */
	STATE_READ_TOKEN,    				/* send at read for data */
	STATE_QHTTPURL1,					/* configure url for http server */
	STATE_QHTTPURL_CONTENT1,			/* configure url add for http server */
	STATE_QHTTPPOST1,					/* send post request */
	STATE_UPLOAD_INFO,	
	STATE_READ_RESULT,
	STATE_QHTTP_VERSION_URL,			/* version url */
	STATE_QHTTP_VERSION_CONTENT,
	STATE_QHTTP_VERSION_GET,
	STATE_READ_VERSION,
	STATE_QHTTP_DOWNLOAD_URL,
	STATE_QHTTP_DOWNLOAD_CONTENT,
	STATE_QHTTP_DOWNLOAD_GET,	
	STATE_READ_DOWNLOAD_URL,
	STATE_QHTTP_DOWNLOAD_FILE_URL,	
	STATE_QHTTP_DOWNLOAD_FILE_CONTENT,
	STATE_QHTTP_DOWNLOAD_FILE_GET,
	STATE_QHTTP_DOWNLOAD_FILE_READ,
	STATE_LENGTH
} nbiot_state_e;

typedef enum
{
  ACTION_SUCCESS= 0x01,  
  ACTION_FAILED,
  ACTION_REPEAT,
  ACTION_WAIT_AGAIN,
  ACTION_COMPLETED,
} action_result;

typedef struct 
{
    nbiot_state_e cur_state;  
    nbiot_state_e next_state;   
    int  try_cnt;              
    int  wait_time;           
	int delay_time;
    int (*action1)(void);      
    action_result (*action2)(const char *command_buffer); 
} nbiot_fsm_state_t;

typedef struct 
{
    nbiot_state_e cur_state;  
    int  try_cnt;              
    int  wait_time;           
	int delay_time;
    int (*action1)(void);      
    action_result (*action2)(const char *command_buffer); 
} at_fsm_state_t;

typedef struct
{
    int cur_state;  
    int trycnt;     
    uint32_t init;  
    const at_fsm_state_t *fsm_state; 
} nbiot_fsm_state_index_t;

#endif

