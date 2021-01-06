
#include "stm32g4xx_hal.h"

typedef enum
{
  //STATE_HAL_RESET= 0x01,  /*reset module*/
	STATE_AT,               /*send at command wait for response ok*/
	STATE_CPIN,             /*query SIM card status��if failed to identify sim card in 20s, then reboot the module*/
	STATE_CREG,							/* query CS service, if failed to register on CS domain service in 90s, then reboot the module */
	STATE_CGREG,						/* query PS service, if failed to register on PS domain service in 60s, then reboot the module */
	STATE_QICSGP,						/* configure parameters of a tcp/ip context */
	STATE_QIACT,						/* activate a pdp context */
	STATE_QIACT_RESULT,     /* query pdp context */
	STATE_QHTTPCFG_CONTEXT,	/* configure parameter for http server */
	STATE_QHTTPREQ_HEADER,	/* configure request header */
	//STATE_QHTTPRES_HEADER,	/* configure response header */
	STATE_QHTTPURL,					/* configure url for http server */
	STATE_QHTTPURL_CONTENT,	/* configure url add for http server */
	STATE_QHTTPPOST,				/* send post request */
	STATE_REQUEST_TOKEN,    /* send post for token */
	STATE_READ_TOKEN,    		/* send at read for data */
	STATE_QHTTPURL1,					/* configure url for http server */
	STATE_QHTTPURL_CONTENT1,	/* configure url add for http server */
	STATE_QHTTPPOST1,				/* send post request */
	STATE_UPLOAD_INFO,	
	STATE_READ_RESULT,
	STATE_IDEL,             /*����״̬*/  
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
    nbiot_state_e cur_state;    /*��ǰ״̬*/
    nbiot_state_e next_state;   /*��һ��״̬*/
    int  try_cnt;               /*���Դ���*/
    int  wait_time;             /*�ȴ�ʱ��*/
	int delay_time;
    int (*action1)(void);       /*����1:����AT*/
    action_result (*action2)(const char *command_buffer); /*����2:�жϽ��յ��������Ƿ�ΪOK����δ���յ�*/
} nbiot_fsm_state_t;


typedef struct
{
    int cur_state;  //��ǰ״̬
    int trycnt;     //��ǰ״̬�Ѿ����ԵĴ���
    uint32_t init;  //��ǰ״̬��ִ��action1����ִ��action2
    const nbiot_fsm_state_t *fsm_state; //��ǰ״̬�ǹ��ܲ���
} nbiot_fsm_state_index_t;






void init_at_statemachine(void);
nbiot_fsm_state_index_t* get_current_state_index(void);
void jump_to_next_at_statemachine(void);


