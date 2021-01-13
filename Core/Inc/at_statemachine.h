
#include "stm32g4xx_hal.h"

#include "at_define.h"

nbiot_fsm_state_index_t* get_current_state_index(void);
void jump_to_next_at_statemachine(void);
void jump_to_next_at_task(void);
void jump_to_init_at_task(void);

void init_at_module(void);

