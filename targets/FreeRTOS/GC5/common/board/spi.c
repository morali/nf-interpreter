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
spi_t s_spi2;
spi_t s_spi3;

void SPI3_MasterHandler(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData)
{

	/* SPI ring transfer, reads digital inputs and sets digital and analog outputs (PWMs) */
	LPSPI_MasterTransferNonBlocking(LPSPI3, &s_spi3.masterHandle, &s_spi3.spi_transfer);
	
	(void) base;
	(void) handle;
	(void) status;
	(void) userData;
}



void SPI_InitPeripheral(void) {
	const gpio_pin_config_t gpioConfig = {
		.direction        = kGPIO_DigitalOutput,
		.outputLogic      = 0,
		.interruptMode    = kGPIO_NoIntmode
		};

	/* Setup SPI2 */
	GPIO_PinInit(GPIO4, 1, &gpioConfig);  // LPSPI2_CS0 
	GPIO_PinInit(GPIO1, 12, &gpioConfig); // Serial Shift / Parallel Load" pin

	NVIC_SetPriority(LPSPI2_IRQn, LPSPI2_IRQ_PRIO);

	LPSPI_MasterGetDefaultConfig(&s_spi2.masterConfig);
	s_spi2.masterConfig.baudRate = LPSPI2_BAUDRATE; /*! Transfer baudrate - 1MHz */

	LPSPI_RTOS_Init(&s_spi2.masterRtosHandle, LPSPI2, &s_spi2.masterConfig, LPSPI2_CLOCK_FREQUENCY);

	/* Setup SPI3 */

	NVIC_SetPriority(LPSPI3_IRQn, LPSPI3_IRQ_PRIO);

	LPSPI_MasterGetDefaultConfig(&s_spi3.masterConfig);
	s_spi3.masterConfig.baudRate = LPSPI3_BAUDRATE;
	s_spi3.masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / LPSPI3_BAUDRATE * 2;
    s_spi3.masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / LPSPI3_BAUDRATE * 2;
    s_spi3.masterConfig.betweenTransferDelayInNanoSec = 1000000000 / LPSPI3_BAUDRATE * 2;
	s_spi3.masterConfig.bitsPerFrame = 24;

	LPSPI_MasterInit(LPSPI3, &s_spi3.masterConfig, LPSPI3_CLOCK_FREQUENCY);
	LPSPI_EnableInterrupts(LPSPI3, kLPSPI_TransferCompleteInterruptEnable);
	 /* Enable at the NVIC */
    EnableIRQ(LPSPI3_IRQn);

	LPSPI_MasterTransferCreateHandle(LPSPI3, &s_spi3.masterHandle, SPI3_MasterHandler, NULL);
}

