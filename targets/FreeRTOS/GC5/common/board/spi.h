/*
 * spi.h
 *
 *  Created on: 09.08.2019
 *      Author: Jakub Standarski
 */

#ifndef SPI_H_
#define SPI_H_

#include "fsl_gpio.h"
#include "fsl_lpspi_freertos.h"


#define LPSPI2_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI2_BAUDRATE 1000000U
#define LPSPI2_IRQ_PRIO 5U
#define LPSPI2_CLOCK_FREQUENCY                                                 \
  (CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / (LPSPI2_CLOCK_SOURCE_DIVIDER + 1U))

#define LPSPI3_BUFFSIZE 3U
#define LPSPI3_BAUDRATE 5000000U
#define LPSPI3_IRQ_PRIO 5U
#define LPSPI3_CLOCK_FREQUENCY                                                 \
  (CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / (LPSPI2_CLOCK_SOURCE_DIVIDER + 1U))

typedef struct _spi {
  lpspi_master_config_t masterConfig;
  lpspi_master_handle_t masterHandle;
  lpspi_rtos_handle_t masterRtosHandle;
  lpspi_transfer_t spi_transfer;
} spi_t;

extern spi_t s_spi2;
extern spi_t s_spi3;

void SPI_InitPeripheral(void);

#endif /* SPI_H_ */