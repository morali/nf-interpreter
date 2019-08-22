/*
 * Created on Wed Aug 21 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "FreeRTOSCommonHooks.h"
#include "FreeRTOS.h"
#include "MIMXRT1062.h"
#include "task.h"

// Need to have calls to these two functions in C code.
// Because they are called only on asm code, GCC linker with LTO option thinks they are not used and just removes them.
// Having them called from a dummy function that is never called it a workaround for this.
// The clean alternative would be to add the GCC attribute used in those functions, but that's not our code to touch.

void dummyFunction(void) __attribute__((used));

// Never called.
void dummyFunction(void) {
  vTaskSwitchContext();
}

//Handle FreeRTOS Stack Overflow
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pxTask;
  (void)pcTaskName;

  taskDISABLE_INTERRUPTS();

  // forces a breakpoint causing the debugger to stop
  // if no debugger is attached this is ignored
  __BKPT(0);

  // If no debugger connected, just reset the board
  NVIC_SystemReset();
}

/* FreeRTOS application idle hook */
void vApplicationIdleHook(void) {
  /* Best to sleep here until next systick */
  __DSB();
  __WFI();
  __ISB();
}

/* FreeRTOS malloc fail hook */
void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  // forces a breakpoint causing the debugger to stop
  // if no debugger is attached this is ignored
  __BKPT(0);

  // If no debugger connected, just reset the board
  NVIC_SystemReset();
}

/* Delay for the specified number of milliSeconds */
void FreeRTOSDelay(uint32_t ms)
{
	TickType_t xDelayTime= xTaskGetTickCount();

  TickType_t xMs = pdMS_TO_TICKS(ms);
	vTaskDelayUntil(&xDelayTime, xMs);
}