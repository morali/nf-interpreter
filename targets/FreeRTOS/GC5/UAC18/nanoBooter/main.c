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

#include "FreeRTOS.h"
#include "task.h"

#include <targetHAL.h>
#include <WireProtocol_ReceiverThread.h>
#include <nanoPAL_BlockStorage.h>
#include "nanoHAL_ConfigurationManager.h"
#include "Target_BlockStorage_iMXRTFlashDriver.h"

#include "usb.h"
#include "usb_vcom.h"

extern usb_cdc_vcom_struct_t s_cdcVcom;

//configure heap memory
__attribute__((section(".noinit.$SRAM_OC.ucHeap")))
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

#define LED_GPIO GPIO1
#define LED_GPIO_PIN (8U)

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

static void blink_task(void *pvParameters)
{
    (void)pvParameters;

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /* Init output LED GPIO. */
    GPIO_PinInit(LED_GPIO, LED_GPIO_PIN, &led_config);

    for (;;)
    {
        vTaskDelay(100);
        GPIO_PortToggle(LED_GPIO, 1u << LED_GPIO_PIN);
        vTaskDelay(500);
        GPIO_PortToggle(LED_GPIO, 1u << LED_GPIO_PIN);
    }
}

static void  boot_nanoCLR(void){
    extern uint32_t __nanoCLR_start__;
    uint32_t button = 0;

    /* Define the init structure for the output TP6 pin*/
    gpio_pin_config_t tp6_config = {
        .direction      = kGPIO_DigitalOutput, 
        .outputLogic    = 0, 
        .interruptMode  = kGPIO_NoIntmode
    };

    /* Init input GPIO*/
    GPIO_PinInit(BOARD_USER_TP6_GPIO, BOARD_USER_TP6_GPIO_PIN, &tp6_config);

    /* Define the init structure for the input TP7 pin*/
    gpio_pin_config_t button_config = {
        .direction      = kGPIO_DigitalInput, 
        .outputLogic    = 0, 
        .interruptMode  = kGPIO_NoIntmode
    };

    /* Init input GPIO*/
    GPIO_PinInit(BOARD_USER_TP7_GPIO, BOARD_USER_TP7_GPIO_PIN, &button_config);
    button = GPIO_PinRead(BOARD_USER_TP7_GPIO, BOARD_USER_TP7_GPIO_PIN);

    /* Button is active low.
       Load nanoCLR program through resetISR or if button is pressed init reciver Task */
    if (button) 
    {
        void (*nanoCLR)(void);
        nanoCLR = (void *) *(&__nanoCLR_start__ + 1); // resetISR address
        nanoCLR();
    } 
}

int main(void)
{

    //delay for development purposes
    // for (volatile uint32_t i = 0; i < 100000000; i++) {
    //     __asm("nop");
    // }

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    
    //SCB_DisableDCache();

    boot_nanoCLR();

    iMXRTFlexSPIDriver_InitializeDevice(NULL);
    
    // initialize block storage list and devices
    // in CLR this is called in nanoHAL_Initialize()
    // for nanoBooter we have to init it in order to provide the flash map for Monitor_FlashSectorMap command
    BlockStorageList_Initialize();
    BlockStorage_AddDevices();

    // initialize configuration manager
    // in CLR this is called in nanoHAL_Initialize()
    // for nanoBooter we have to init it here to have access to network configuration blocks
    ConfigurationManager_Initialize();  
    
    xTaskCreate(blink_task, "blink_task", configMINIMAL_STACK_SIZE + 10, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(ReceiverThread, "ReceiverThread", 2048, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();

    for (;;)
        ;
    return 0;
}