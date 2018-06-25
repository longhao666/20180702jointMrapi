#ifndef _CAN_STM32_H_
#define _CAN_STM32_H_
#include "stm32f4xx.h"
#include "sys.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef int TASK_HANDLE;
typedef CAN_HandleTypeDef* CAN_HANDLE;
#define CAN_NONEBUS NULL

#ifdef __cplusplus
}
#endif

#endif
