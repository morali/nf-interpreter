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

/************************************************************************************************************/
/*                                                                                                          */
/*                                      SPI STRUCTURES DECLARATIONS                                         */
/*                                                                                                          */
/************************************************************************************************************/

typedef struct {
  lpspi_master_config_t masterConfig;
  lpspi_rtos_handle_t masterRtosHandle;
} spi_t;

extern spi_t spi2;

/************************************************************************************************************/
/*                                                                                                          */
/*                                   SPI PUBLIC FUNCTIONS DECLARATIONS                                      */
/*                                                                                                          */
/************************************************************************************************************/

void SPI2_InitPeripheral(void);

#endif /* SPI_H_ */