
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#define GPS_DATA_FLAG	1U
#define AT_DATA_FLAG	1U
#define AT_IDLE_FLAG	2U
#define AT_RESET_FLAG	4U
#define AT_WAIT_FLAG	(AT_DATA_FLAG|AT_IDLE_FLAG|AT_RESET_FLAG)
extern osThreadId_t gps_task_handle;
extern osThreadId_t at_task_handle;

void reset_at_module(void);
