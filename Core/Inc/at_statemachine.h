
#include "stm32g4xx_hal.h"

typedef enum
{
  //STATE_HAL_RESET= 0x01,  /*reset module*/
  STATE_AT,               /*send at command wait for response ok*/
	STATE_CPIN,             /*query SIM card status，if failed to identify sim card in 20s, then reboot the module*/
	STATE_CREG,							/* query CS service, if failed to register on CS domain service in 90s, then reboot the module */
	STATE_CGREG,						/* query PS service, if failed to register on PS domain service in 60s, then reboot the module */
	STATE_QICSGP,						/* configure parameters of a tcp/ip context */
	STATE_QIACT,						/* activate a pdp context */
  STATE_QIACT_RESULT,     /* query pdp context */
	STATE_QHTTPCFG_CONTEXT,	/* configure parameter for http server */
	STATE_QHTTPREQ_HEADER,	/* configure request header */
	STATE_QHTTPRES_HEADER,	/* configure response header */
	STATE_QHTTPURL,					/* configure url for http server */
	STATE_QHTTPURL_CONTENT,	/* configure url add for http server */
	STATE_QHTTPPOST,				/* send post request */
	STATE_REQUEST_TOKEN,    /* send post for token */
	STATE_READ_TOKEN,    		/* send at read for data */
  STATE_SEND_DATA,        /*  */
  STATE_IDEL,             /*空闲状态*/  
	STATE_LENGTH,
} nbiot_state_e;

typedef enum
{
  ACTION_SUCCESS= 0x01,  
  ACTION_FAILED,
	ACTION_REPEAT,
	ACTION_WAIT_AGAIN,
	ACTION_RESULT_LENGTH,
} action_result;

typedef struct 
{
    nbiot_state_e cur_state;    /*当前状态*/
    nbiot_state_e next_state;   /*下一个状态*/
    int  try_cnt;               /*重试次数*/
    int  wait_time;             /*等待时间*/
    int (*action1)(void);       /*动作1:发送AT*/
    action_result (*action2)(const char *command_buffer); /*动作2:判断接收到的数据是否为OK或者未接收到*/
} nbiot_fsm_state_t;


typedef struct
{
    int cur_state;  //当前状态
    int trycnt;     //当前状态已经重试的次数
    uint32_t init;  //当前状态是执行action1还是执行action2
    const nbiot_fsm_state_t *fsm_state; //当前状态是功能参数
} nbiot_fsm_state_index_t;






void init_at_statemachine(void);
nbiot_fsm_state_index_t* get_current_state_index(void);
void jump_to_next_at_statemachine(void);


