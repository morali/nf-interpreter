//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "MIMXRT1062.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include "pin_mux.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOSCommonHooks.h"
#include "task.h"

#include "Target_BlockStorage_iMXRTFlashDriver.h"
#include "nanoHAL_ConfigurationManager.h"
#include <WireProtocol_ReceiverThread.h>
#include <nanoPAL_BlockStorage.h>
#include <targetHAL.h>

#include "usb_vcom.h"
#include "i2c.h"
#include "GlobalEventsFlags.h"
#include "MAC_address.h"

//configure heap memory
__attribute__((section(".noinit.$SRAM_OC.ucHeap")))
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

#define LED_GPIO GPIO1
#define LED_GPIO_PIN (8U)

static void vBlinkTask(void *pvParameters) {
  (void)pvParameters;

  /* Define the init structure for the output LED pin*/
  gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

  /* Init output LED GPIO. */
  GPIO_PinInit(LED_GPIO, LED_GPIO_PIN, &led_config);

  for (;;) {
    FreeRTOSDelay(100);
    GPIO_PortToggle(LED_GPIO, 1u << LED_GPIO_PIN);
    FreeRTOSDelay(500);
    GPIO_PortToggle(LED_GPIO, 1u << LED_GPIO_PIN);
  }
}

static void boot_nanoCLR(void) {
  extern uint32_t __nanoCLR_start__;
  uint32_t button = 0;

  /* Define the init structure for the output TP6 pin*/
  gpio_pin_config_t tp6_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0,
      .interruptMode = kGPIO_NoIntmode};

  /* Init input GPIO*/
  GPIO_PinInit(BOARD_USER_TP6_GPIO, BOARD_USER_TP6_GPIO_PIN, &tp6_config);

  /* Define the init structure for the input TP7 pin*/
  gpio_pin_config_t button_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0,
      .interruptMode = kGPIO_NoIntmode};

  /* Init input GPIO*/
  GPIO_PinInit(BOARD_USER_TP7_GPIO, BOARD_USER_TP7_GPIO_PIN, &button_config);
  button = GPIO_PinRead(BOARD_USER_TP7_GPIO, BOARD_USER_TP7_GPIO_PIN);

  /* Button is active low.
       Load nanoCLR program through resetISR or if button is pressed init reciver Task */
  if (button) {
    void (*nanoCLR)(void);
    nanoCLR = (void *)*(&__nanoCLR_start__ + 1); // resetISR address
    nanoCLR();
  }
}

void vInitializeConfigurationManager(void *params) {
  (void)params;

  // initialize configuration manager
  // in CLR this is called in nanoHAL_Initialize()
  // for nanoBooter we have to init it here to have access to network configuration blocks
  ConfigurationManager_Initialize();

  vTaskDelete(NULL);
}

int main(void) {

  //delay for development purposes
  for (volatile uint32_t i = 0; i < 100000000; i++) {
    __asm("nop");
  }

  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();

  //Flash Init shoud be done ASAP - don't move this function
  iMXRTFlexSPIDriver_InitializeDevice(NULL);

  //SCB_DisableDCache();

  boot_nanoCLR();

  I2C3_InitPeripheral();
  USB_Init();

  // initialize block storage list and devices
  // in CLR this is called in nanoHAL_Initialize()
  // for nanoBooter we have to init it in order to provide the flash map for Monitor_FlashSectorMap command
  BlockStorageList_Initialize();
  BlockStorage_AddDevices();
  GlobalEventsFlags_Init();

  xTaskCreate(vMacAddressThread, "MacAddressThread", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 13, NULL);
  xTaskCreate(vInitializeConfigurationManager, "InitCMTask", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 13, NULL);
  xTaskCreate(vBlinkTask, "BlinkTask", configMINIMAL_STACK_SIZE + 10, NULL, configMAX_PRIORITIES - 15, NULL);
  xTaskCreate(vReceiverThread, "ReceiverThread", 2048, NULL, configMAX_PRIORITIES - 15, NULL);

  vTaskStartScheduler();

  for (;;)
    ;
  return 0;
}