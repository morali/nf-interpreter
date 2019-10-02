/*
 * spi.h
 *
 *  Created on: 09.08.2019
 *      Author: Jakub Standarski
 */

#ifndef SPI_H_
#define SPI_H_

#include "fsl_lpspi_freertos.h"

#define LPSPI2_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI2_BAUDRATE 1000000U
#define LPSPI2_IRQ_PRIO 7U
#define LPSPI2_CLOCK_FREQUENCY                                                 \
  (CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / (LPSPI2_CLOCK_SOURCE_DIVIDER + 1U))

#define LPSPI3_BUFFSIZE 3U
#define LPSPI3_BAUDRATE 5000000U
#define LPSPI3_IRQ_PRIO 7U
#define LPSPI3_CLOCK_FREQUENCY                                                 \
  (CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / (LPSPI2_CLOCK_SOURCE_DIVIDER + 1U))

void SPI_InitPeripheral(void);
lpspi_transfer_t *GetSpi3Transfer();
lpspi_master_handle_t *GetSpi3MasterHandle();
lpspi_rtos_handle_t *GetSpi2MasterRtosHandle();

#endif /* SPI_H_ */