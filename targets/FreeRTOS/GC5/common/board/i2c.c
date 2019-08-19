/*
 * i2c.c
 *
 *  Created on: 01.08.2019
 *      Author: Jakub Standarski
 */

#include "i2c.h"

/////////////////////////////////
// I2C CONFIGURATION STRUCTURE //
//      DECLARED IN i2c.h      //
/////////////////////////////////
i2c_t i2c3;




/************************************************************************************************************/
/*                                                                                                          */
/*                                            I2C DEFINES                                                   */
/*                                                                                                          */
/************************************************************************************************************/

/* Clock divider for master lpi2c clock source */
#define LPI2C3_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C3_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C3_CLOCK_SOURCE_DIVIDER + 1U))




/************************************************************************************************************/
/*                                                                                                          */
/*                                  I2C PUBLIC FUNCTIONS DEFINITIONS                                        */
/*                                                                                                          */
/************************************************************************************************************/

void I2C3_InitPeripheral(void)
{
    NVIC_SetPriority(LPI2C3_IRQn, 3U);

    LPI2C_MasterGetDefaultConfig(&i2c3.masterConfig);

    LPI2C_RTOS_Init(&i2c3.masterRtosHandle, LPI2C3, &i2c3.masterConfig, LPI2C3_CLOCK_FREQUENCY);
}



void I2C_MasterStructureInit(lpi2c_master_transfer_t *pTransfer, uint16_t slaveAddress, uint32_t subaddress,
                             size_t subaddressSize, uint32_t flags)
{
    pTransfer->slaveAddress   = slaveAddress;
    pTransfer->subaddress     = subaddress;
    pTransfer->subaddressSize = subaddressSize;
    pTransfer->flags          = flags;
}                            



void I2C_SetBufferAndDirection(lpi2c_master_transfer_t *pTransfer, uint8_t *buffer, size_t dataSize, lpi2c_direction_t direction)
{
    pTransfer->data      = buffer;
    pTransfer->dataSize  = dataSize;
    pTransfer->direction = direction;
}