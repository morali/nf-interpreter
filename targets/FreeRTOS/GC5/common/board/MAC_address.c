/*
 * MAC_address.c
 *
 *  Created on: 02.08.2019
 *      Author: Jakub Standarski
 */

#include "FreeRTOSCommonHooks.h"
#include "GlobalEventsFlags.h"
#include "i2c.h"
#include "task.h"
#include "time.h"

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
#define MAC_ADDRESS         0x50
#define MAC_SUBADDRESS      0xFA
#define MAC_SUBADDRESS_SIZE 0x01
#define MAC_DATA_SIZE       0x06

/************************************************************************************************************/
/*                                                                                                          */
/*                                MAC ADDRESS PUBLIC FUNCTIONS DEFINITIONS                                  */
/*                                                                                                          */
/************************************************************************************************************/

uint8_t *MAC_GetAddress() {
  xEventGroupWaitBits(xGlobalEventsFlags, MAC_ADDRESS_READ, pdFALSE, pdTRUE, portMAX_DELAY);

  return macAddress;
}

/************************************************************************************************************/
/*                                                                                                          */
/*                                           FreeRTOS TASK                                                  */
/*                                                                                                          */
/************************************************************************************************************/

void vMacAddressThread(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    status_t status = I2CTransfer(&i2c3, MAC_ADDRESS, kLPI2C_Read, MAC_SUBADDRESS, MAC_SUBADDRESS_SIZE, macAddress, MAC_DATA_SIZE);
    if (status == kStatus_Success) {
      xEventGroupSetBits(xGlobalEventsFlags, MAC_ADDRESS_READ);
      vTaskDelete(NULL);
    } else {
      FreeRTOSDelay(100);
    }
  }
}