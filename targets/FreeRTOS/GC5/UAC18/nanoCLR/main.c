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
#include "i2c.h"

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
#include "LocalIO.h"

#include "usb_vcom.h"

#include "log_native.h"
#include "BACnetThread.h"

//configure heap memory
__attribute__((section(".noinit.$SRAM_OC.ucHeap")))
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

// override malloc and free functions with own allocator
void *malloc(size_t sz) { return platform_malloc(sz); }
void free(void *p) { platform_free(p); }

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    
    //Flash Init shoud be done ASAP - don't move this function
    iMXRTFlexSPIDriver_InitializeDevice(NULL);

    BOARD_InitHyperRAM();
    I2C3_InitPeripheral();
    I2C2_InitPeripheral();
    SPI_InitPeripheral();
    GlobalEventsFlags_Init();
    USB_Init();
    //SCB_DisableDCache();

    addChannel("Firmware", Troubleshot);
    addLog("Firmware", Troubleshot, "Starting iSMA-CM-UAC18");
    addLog("Firmware", Troubleshot, "Build "__DATE__" "__TIME__);


    xTaskCreate(CLRStartupThread, "CLRStartupThread", 8192, NULL, configMAX_PRIORITIES - 15, NULL);
    xTaskCreate(SdCardThread, "SDCardThread", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 15, NULL);
    xTaskCreate(ReceiverThread, "ReceiverThread", 2048, NULL, configMAX_PRIORITIES - 14, NULL);
    xTaskCreate(vRtcThread, "RtcThread", configMINIMAL_STACK_SIZE + 32, NULL, configMAX_PRIORITIES - 13, NULL);
    xTaskCreate(vMacAddressThread, "MacAddressThread", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 13, NULL);
    xTaskCreate(vPanelThread, "PanelThread", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 14, NULL);
    xTaskCreate(vLocalIOThread, "LocalIOhread", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 14, NULL);
    xTaskCreate(vBACnetThread, "bacnet", configMINIMAL_STACK_SIZE + 2048, NULL, (configMAX_PRIORITIES  - 14), NULL);
    
    addLog("Firmware", Troubleshot, "Start system");
    vTaskStartScheduler();

    for (;;)
        ;
    return 0;
}
