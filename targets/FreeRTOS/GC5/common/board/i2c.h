/*
 * i2c.h
 *
 *  Created on: 01.08.2019
 *      Author: Jakub Standarski
 */

#ifndef I2C_H_
#define I2C_H_

#include "fsl_lpi2c_freertos.h"

/************************************************************************************************************/
/*                                                                                                          */
/*                                      I2C STRUCTURES DECLARATIONS                                         */
/*                                                                                                          */
/************************************************************************************************************/

typedef struct
{
  lpi2c_master_config_t masterConfig;
  lpi2c_rtos_handle_t masterRtosHandle;
} i2c_t;

extern i2c_t i2c3;
extern i2c_t i2c2;

/************************************************************************************************************/
/*                                                                                                          */
/*                                   I2C PUBLIC FUNCTIONS DECLARATIONS                                      */
/*                                                                                                          */
/************************************************************************************************************/

void I2C3_InitPeripheral(void);
void I2C2_InitPeripheral(void);

/**
 * @brief  Do transfer via I2c
 * @note   
 * @param  *pi2c: Pointer to i2c_t structure
 * @param  dev_addr: Device Address
 * @param  direction: Direction Read or Write
 * @param  subaddress: 
 * @param  subaddressSize: 
 * @param  *buff: Pointer to data to write or buffer where to read data
 * @param  size: number of bytes to write or read
 * @retval transfer status
 */
status_t I2CTransfer(i2c_t *pi2c, uint8_t dev_addr, lpi2c_direction_t direction, uint32_t subaddress, uint8_t subaddressSize, uint8_t *buff, size_t size);

#endif /* I2C_H_ */