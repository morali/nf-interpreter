/*
 * Created on Wed Aug 21 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef __FREERTOSCOMMONHOOKS_H_
#define __FREERTOSCOMMONHOOKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/**
 * @brief  Delay for the specified number of milliSeconds
 * @note   Delays the specified number of milliSeoconds using a task delay
 * @param  ms: Delay in milliSeconds
 * @retval None
 */
void FreeRTOSDelay(uint32_t ms);

/**
 * @brief  FreeRTOS malloc fail hook
 * @note   This function is called when a malloc fails to allocate data.
 * @retval None
 */
void vApplicationMallocFailedHook(void);

/**
 * @brief  FreeRTOS application idle hook
 * @note   Calls ARM Wait for Interrupt function to idle core
 * @retval None
 */
void vApplicationIdleHook(void);

/**
 * @brief  FreeRTOS stack overflow hook
 * @note   This function is alled when a stack overflow occurs.
 * @param  pxTask: Task handle that overflowed stack
 * @param  *pcTaskName: Task name that overflowed stack
 * @retval None
 */
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);

#endif /* __FREERTOSCOMMONHOOKS_H_ */