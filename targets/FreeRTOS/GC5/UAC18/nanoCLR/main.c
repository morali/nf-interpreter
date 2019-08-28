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
// #include "spi.h"

#include "FreeRTOS.h"
#include "task.h"

#include <nanoHAL_v2.h>
#include <Target_Windows_Storage.h>

#include <WireProtocol_ReceiverThread.h>
#include <nanoCLR_Application.h>
#include "Target_BlockStorage_iMXRTFlashDriver.h"
#include "CLR_Startup_Thread.h"

// #include "External_RTC.h"
// #include "MAC_address.h"
#include "GlobalEventsFlags.h"
// #include "Panel.h"
#include "isma_localio.h"


//configure heap memory
__attribute__((section(".noinit.$SRAM_OC.ucHeap")))
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    //Flash Init shoud be done ASAP - don't move this function
    iMXRTFlexSPIDriver_InitializeDevice(NULL);

    BOARD_InitHyperRAM();
    // I2C3_InitPeripheral();
    SPI_InitPeripheral();
    GlobalEventsFlags_Init();
    // SCB_DisableDCache();

    
    xTaskCreate(CLRStartupThread, "CLRStartupThread", 8192, NULL, configMAX_PRIORITIES - 15, NULL);
    xTaskCreate(SdCardThread, "SDCardThread", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 15, NULL);
    xTaskCreate(ReceiverThread, "ReceiverThread", 2048, NULL, configMAX_PRIORITIES - 14, NULL);
    // xTaskCreate(vRtcThread, "RtcThread", configMINIMAL_STACK_SIZE + 16, NULL, configMAX_PRIORITIES - 13, NULL);
    // xTaskCreate(vMacAddressThread, "MacAddressThread", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 13, NULL);
    // xTaskCreate(vPanelThread, "PanelThread", configMINIMAL_STACK_SIZE +512, NULL, configMAX_PRIORITIES - 14, NULL);
    xTaskCreate(vLocalIOThread, "LocalIOhread", configMINIMAL_STACK_SIZE + 512, NULL, configMAX_PRIORITIES - 13, NULL);
    
    vTaskStartScheduler();

    for (;;)
        ;
    return 0;
}
