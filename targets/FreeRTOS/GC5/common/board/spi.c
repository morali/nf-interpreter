/*
 * spi.c
 *
 *  Created on: 09.08.2019
 *      Author: Jakub Standarski
 */

#include "spi.h"
#include "fsl_gpio.h"

static lpspi_transfer_t spi3_transfer;
static lpspi_master_handle_t spi3_masterHandle;
static lpspi_rtos_handle_t masterRtosHandle;

lpspi_transfer_t *GetSpi3Transfer() {
	return &spi3_transfer;
}

lpspi_master_handle_t *GetSpi3MasterHandle() {
	return &spi3_masterHandle;
}

lpspi_rtos_handle_t *GetSpi2MasterRtosHandle() {
	return &masterRtosHandle;
}

void SPI3_MasterHandler(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData) {
  /* SPI ring transfer, reads digital inputs and sets digital and analog outputs (PWMs) */
  LPSPI_MasterTransferNonBlocking(LPSPI3, &spi3_masterHandle, &spi3_transfer);

  (void)base;
  (void)handle;
  (void)status;
  (void)userData;
}

void SPI_InitPeripheral(void) {
	lpspi_master_config_t masterConfig;

	const gpio_pin_config_t gpioConfig = {
		.direction        = kGPIO_DigitalOutput,
		.outputLogic      = 0,
		.interruptMode    = kGPIO_NoIntmode
		};

	/* Setup SPI2 */
	GPIO_PinInit(GPIO4, 1, &gpioConfig);  // LPSPI2_CS0 
	GPIO_PinInit(GPIO1, 12, &gpioConfig); // Serial Shift / Parallel Load" pin

	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate = LPSPI2_BAUDRATE; /*! Transfer baudrate - 1MHz */
	masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / LPSPI2_BAUDRATE * 2;
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / LPSPI2_BAUDRATE * 2;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / LPSPI2_BAUDRATE * 2;
	
	NVIC_SetPriority(LPSPI2_IRQn, LPSPI2_IRQ_PRIO);
	LPSPI_RTOS_Init(&masterRtosHandle, LPSPI2, &masterConfig, LPSPI2_CLOCK_FREQUENCY);

	/* Setup SPI3 */
	LPSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate = LPSPI3_BAUDRATE;
	masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / LPSPI3_BAUDRATE * 2;
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / LPSPI3_BAUDRATE * 2;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / LPSPI3_BAUDRATE * 2;
	masterConfig.bitsPerFrame = 24;

	LPSPI_MasterInit(LPSPI3, &masterConfig, LPSPI3_CLOCK_FREQUENCY);
	NVIC_SetPriority(LPSPI3_IRQn, LPSPI3_IRQ_PRIO);
	LPSPI_EnableInterrupts(LPSPI3, kLPSPI_TransferCompleteInterruptEnable);
	 /* Enable at the NVIC */
    EnableIRQ(LPSPI3_IRQn);

	LPSPI_MasterTransferCreateHandle(LPSPI3, &spi3_masterHandle, SPI3_MasterHandler, NULL);
}

