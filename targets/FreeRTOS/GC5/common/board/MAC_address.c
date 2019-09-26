/*
 * Created on Thu Sep 26 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
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

/* MAC Address generic defines */
#define MAC_ADDRESS         0x50
#define MAC_SUBADDRESS      0xFA
#define MAC_SUBADDRESS_SIZE 0x01
#define MAC_DATA_SIZE       0x06

uint8_t *MAC_GetAddress() {
  xEventGroupWaitBits(xGlobalEventsFlags, MAC_ADDRESS_READ, pdFALSE, pdTRUE, portMAX_DELAY);

  return macAddress;
}

void vMacAddressThread(void *pvParameters) {
  (void)pvParameters;
  lpi2c_rtos_handle_t *i2c3_masterRtosHandle = GetI2C3_Handle();
  for (;;) {
    status_t status = I2CTransfer(i2c3_masterRtosHandle, MAC_ADDRESS, kLPI2C_Read, MAC_SUBADDRESS, MAC_SUBADDRESS_SIZE, macAddress, MAC_DATA_SIZE);
    if (status == kStatus_Success) {
      xEventGroupSetBits(xGlobalEventsFlags, MAC_ADDRESS_READ);
      vTaskDelete(NULL);
    } else {
      FreeRTOSDelay(100);
    }
  }
}