/*
 * Created on Thu Sep 26 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef I2C_H_
#define I2C_H_

#include "fsl_lpi2c_freertos.h"

void I2C3_InitPeripheral(void);
void I2C2_InitPeripheral(void);

/**
 * @brief  Get handles to I2C transfer structure
 * @note   
 * @retval 
 */
lpi2c_rtos_handle_t *GetI2C2_Handle();
lpi2c_rtos_handle_t *GetI2C3_Handle();

/**
 * @brief  Do transfer via I2c
 * @note   
 * @param  *pi2c_handle: Pointer to lpi2c_rtos_handle_t structure
 * @param  dev_addr: Device Address
 * @param  direction: Direction Read or Write
 * @param  subaddress: 
 * @param  subaddressSize: 
 * @param  *buff: Pointer to data to write or buffer where to read data
 * @param  size: number of bytes to write or read
 * @retval transfer status
 */
status_t I2CTransfer(lpi2c_rtos_handle_t *pi2c_handle, uint8_t dev_addr, lpi2c_direction_t direction, uint32_t subaddress, uint8_t subaddressSize, uint8_t *buff, size_t size);

#endif /* I2C_H_ */