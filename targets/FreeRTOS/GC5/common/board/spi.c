/*
 * spi.c
 *
 *  Created on: 09.08.2019
 *      Author: Jakub Standarski
 */

#include "spi.h"

/////////////////////////////////
// SPI CONFIGURATION STRUCTURE //
//      DECLARED IN spi.h      //
/////////////////////////////////
spi_t spi2;

/************************************************************************************************************/
/*                                                                                                          */
/*                                         SPI PRIVATE DEFINES                                              */
/*                                                                                                          */
/************************************************************************************************************/

#define LPSPI2_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI2_CLOCK_FREQUENCY                                                 \
  (CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / (LPSPI2_CLOCK_SOURCE_DIVIDER + 1U))

/************************************************************************************************************/
/*                                                                                                          */
/*                                  SPI PUBLIC FUNCTIONS DEFINITIONS                                        */
/*                                                                                                          */
/************************************************************************************************************/

void SPI2_InitPeripheral(void) {
  const gpio_pin_config_t gpioConfig = {
      .direction        = kGPIO_DigitalOutput,
      .outputLogic      = 0,
      .interruptMode    = kGPIO_NoIntmode
    };

  GPIO_PinInit(GPIO4, 1, &gpioConfig);  // LPSPI2_CS0 
  GPIO_PinInit(GPIO1, 12, &gpioConfig); // Serial Shift / Parallel Load" pin

  NVIC_SetPriority(LPSPI2_IRQn, 3U);

  LPSPI_MasterGetDefaultConfig(&spi2.masterConfig);
  spi2.masterConfig.baudRate = 1000000U; /*! Transfer baudrate - 1MHz */

  LPSPI_RTOS_Init(&spi2.masterRtosHandle, LPSPI2, &spi2.masterConfig, LPSPI2_CLOCK_FREQUENCY);
}