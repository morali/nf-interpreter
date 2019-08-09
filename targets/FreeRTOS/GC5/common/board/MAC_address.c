/*
 * MAC_address.c
 *
 *  Created on: 02.08.2019
 *      Author: Jakub Standarski
 */

#include "MAC_address.h"




///////////////////////////////////////////
//   MAC address private declarations    //
///////////////////////////////////////////
static uint8_t macAddress[6];




/************************************************************************************************************/
/*                                                                                                          */
/*                                        MAC ADDRESS DEFINES                                               */
/*                                                                                                          */
/************************************************************************************************************/

/* MAC Address generic defines */
#define MAC_I2C3_ADDRESS        (uint16_t)0x50
#define MAC_SUBADDRESS          (uint32_t)0xFA
#define MAC_SUBADDRESS_SIZE     (size_t)0x01
#define MAC_DATA_SIZE           (size_t)0x06




/************************************************************************************************************/
/*                                                                                                          */
/*                                MAC ADDRESS PUBLIC FUNCTIONS DEFINITIONS                                  */
/*                                                                                                          */
/************************************************************************************************************/

uint8_t *MAC_GetAddress()
{   
    xEventGroupWaitBits(xGlobalEventsFlags, MAC_ADDRESS_READ , pdFALSE, pdTRUE, portMAX_DELAY);
    
    return macAddress;    
}




/************************************************************************************************************/
/*                                                                                                          */
/*                                           FreeRTOS TASK                                                  */
/*                                                                                                          */
/************************************************************************************************************/

void vMacAddressThread(void *pvParameters)
{
    (void)pvParameters;
    
    status_t macAddressRead = kStatus_Fail;
    lpi2c_master_transfer_t macAddressTransfer;
    I2C_MasterStructureInit(&macAddressTransfer, MAC_I2C3_ADDRESS, MAC_SUBADDRESS, MAC_SUBADDRESS_SIZE, DEFAULT_FLAG);
    I2C_SetBufferAndDirection(&macAddressTransfer, macAddress, MAC_DATA_SIZE, READ);

    for(;;)
    {
        macAddressRead = LPI2C_RTOS_Transfer(&i2c3.masterRtosHandle, &macAddressTransfer);
        if(macAddressRead == kStatus_Success)
        {          
            xEventGroupSetBits(xGlobalEventsFlags, MAC_ADDRESS_READ);  
            vTaskDelete(NULL);   
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        
    }
    
}