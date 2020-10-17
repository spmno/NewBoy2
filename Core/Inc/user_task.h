
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#define GPS_DATA_FLAG 1U
#define AT_DATA_FLAG 1U
extern osThreadId_t gps_task_handle;
extern osThreadId_t at_task_handle;
