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
/*                                            I2C DEFINES                                                   */
/*                                                                                                          */
/************************************************************************************************************/

/* I2C read/write flags */
#define READ     kLPI2C_Read
#define WRITE    kLPI2C_Write

/* I2C default transfer flag */
#define DEFAULT_FLAG kLPI2C_TransferDefaultFlag




/************************************************************************************************************/
/*                                                                                                          */
/*                                      I2C STRUCTURES DEFINITIONS                                          */
/*                                                                                                          */
/************************************************************************************************************/

typedef struct 
{
    lpi2c_master_config_t   masterConfig;
    lpi2c_rtos_handle_t     masterRtosHandle;

}i2c_t;
    
extern i2c_t i2c3;




/************************************************************************************************************/
/*                                                                                                          */
/*                                   I2C PUBLIC FUNCTIONS DECLARATIONS                                      */
/*                                                                                                          */
/************************************************************************************************************/

void I2C3_InitPeripheral(void);

void I2C_MasterStructureInit(lpi2c_master_transfer_t *pTransfer, uint16_t slaveAddress, uint32_t subaddress,
                             size_t subaddressSize, uint32_t flags);

void I2C_SetBufferAndDirection(lpi2c_master_transfer_t *pTransfer, uint8_t *buffer, size_t dataSize, lpi2c_direction_t direction);





#endif /* I2C_H_ */