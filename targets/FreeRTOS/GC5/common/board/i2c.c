/*
 * Created on Thu Sep 26 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */


#include "i2c.h"

static lpi2c_rtos_handle_t i2c2_masterRtosHandle;
static lpi2c_rtos_handle_t i2c3_masterRtosHandle;

/* Clock divider for master lpi2c clock source */
#define LPI2C3_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C3_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C3_CLOCK_SOURCE_DIVIDER + 1U))

void I2C2_InitPeripheral(void) {
  NVIC_SetPriority(LPI2C2_IRQn, 3U);

  lpi2c_master_config_t masterConfig;
  LPI2C_MasterGetDefaultConfig(&masterConfig);
  masterConfig.hostRequest.source = kLPI2C_HostRequestInputTrigger;
  masterConfig.baudRate_Hz = 200000;

  LPI2C_RTOS_Init(&i2c2_masterRtosHandle, LPI2C2, &masterConfig, LPI2C3_CLOCK_FREQUENCY);
}

lpi2c_rtos_handle_t *GetI2C2_Handle() {
  return &i2c2_masterRtosHandle;
}

void I2C3_InitPeripheral(void) {
  NVIC_SetPriority(LPI2C3_IRQn, 3U);

  lpi2c_master_config_t masterConfig;
  LPI2C_MasterGetDefaultConfig(&masterConfig);
  masterConfig.hostRequest.source = kLPI2C_HostRequestInputTrigger;

  LPI2C_RTOS_Init(&i2c3_masterRtosHandle, LPI2C3, &masterConfig, LPI2C3_CLOCK_FREQUENCY);
}

lpi2c_rtos_handle_t *GetI2C3_Handle() {
  return &i2c3_masterRtosHandle;
}

status_t I2CTransfer(lpi2c_rtos_handle_t *pi2c_handle, uint8_t dev_addr, lpi2c_direction_t direction, uint32_t subaddress, uint8_t subaddressSize, uint8_t *buff, size_t size) {
  lpi2c_master_transfer_t transfer;

  memset(&transfer, 0, sizeof(transfer));
  transfer.slaveAddress = dev_addr;
  transfer.direction = direction;
  transfer.subaddress = subaddress;
  transfer.subaddressSize = subaddressSize;
  transfer.data = buff;
  transfer.dataSize = size;
  transfer.flags = kLPI2C_TransferDefaultFlag;

  status_t status = LPI2C_RTOS_Transfer(pi2c_handle, &transfer);

  return status;
}