//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1062.h"
#include "fsl_debug_console.h"
#include "hyperRAM.h"
#include "spi.h"

#include "FreeRTOS.h"
#include "task.h"

#include <nanoHAL_v2.h>
#include <Target_Windows_Storage.h>

#include <WireProtocol_ReceiverThread.h>
#include <nanoCLR_Application.h>
#include "Target_BlockStorage_iMXRTFlashDriver.h"
#include "CLR_Startup_Thread.h"

#include "External_RTC.h"
#include "MAC_address.h"
#include "GlobalEventsFlags.h"
#include "Panel.h"


//configure heap memory
__attribute__((section(".noinit.$SRAM_OC.ucHeap")))
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

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
  // forces a breakpoint causing the debugger to stop
  // if no debugger is attached this is ignored
  __BKPT(0);

  // If no debugger connected, just reset the board
  NVIC_SystemReset();
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    //Flash Init shoud be done ASAP - don't move this function
    iMXRTFlexSPIDriver_InitializeDevice(NULL);

    BOARD_InitHyperRAM();
    I2C3_InitPeripheral();
    SPI2_InitPeripheral();
    GlobalEventsFlags_Init();
    //SCB_DisableDCache();

    
    xTaskCreate(CLRStartupThread, "CLRStartupThread", 8192, NULL, configMAX_PRIORITIES - 15, NULL);
    xTaskCreate(SdCardThread, "SDCardThread", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 15, NULL);
    xTaskCreate(ReceiverThread, "ReceiverThread", 2048, NULL, configMAX_PRIORITIES - 14, NULL);
    xTaskCreate(vRtcThread, "RtcThread", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 13, NULL);
    xTaskCreate(vMacAddressThread, "MacAddressThread", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 13, NULL);
    xTaskCreate(vPanelThread, "PanelThread", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 14, NULL);
    
    vTaskStartScheduler();

    for (;;)
        ;
    return 0;
}
