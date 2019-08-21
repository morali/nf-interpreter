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
spi_s spi2;
spi_s spi3;

/************************************************************************************************************/
/*                                                                                                          */
/*                                  SPI PUBLIC FUNCTIONS DEFINITIONS                                        */
/*                                                                                                          */
/************************************************************************************************************/

void SPI_InitPeripheral(void) {
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

void SPI3_InitPeripheral(void) {
	const gpio_pin_config_t gpioConfig = {
		.direction        = kGPIO_DigitalOutput,
		.outputLogic      = 0,
		.interruptMode    = kGPIO_NoIntmode
		};

	/* Setup SPI2 */
	GPIO_PinInit(GPIO4, 1, &gpioConfig);  // LPSPI2_CS0 
	GPIO_PinInit(GPIO1, 12, &gpioConfig); // Serial Shift / Parallel Load" pin

	NVIC_SetPriority(LPSPI2_IRQn, LPSPI2_IRQ_PRIO);

	LPSPI_MasterGetDefaultConfig(&spi2.masterConfig);
	spi2.masterConfig.baudRate = LPSPI2_BAUDRATE; /*! Transfer baudrate - 1MHz */

	LPSPI_RTOS_Init(&spi2.masterRtosHandle, LPSPI2, &spi2.masterConfig, LPSPI2_CLOCK_FREQUENCY);

	/* Setup SPI3 */
	GPIO_PinInit(GPIO1, 28, &gpioConfig);

	NVIC_SetPriority(LPSPI3_IRQn, LPSPI3_IRQ_PRIO);

	LPSPI_MasterGetDefaultConfig(&spi3.masterConfig);
	spi3.masterConfig.baudRate = LPSPI3_BAUDRATE;

	LPSPI_RTOS_Init(&spi3.masterRtosHandle, LPSPI3, &spi3.masterConfig, LPSPI3_CLOCK_FREQUENCY);
}