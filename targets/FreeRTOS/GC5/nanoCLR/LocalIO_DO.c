/*
 * Created on Mon Sep 09 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "LocalIO_DO.h"
#include "LocalIO.h"

static local_io_t *local_io_tx;

/**
 * @brief  DO LookUpTable
 * @note   The digital outputs are not connected sequentially so to simplify the code 
 *         and increase its readability, use this table to refer to the corresponding output
 * @retval 
 */
static const uint8_t DO_LUT[DIGITAL_OUTPUT_PORTS] = {(1 << 3), (1 << 4), (1 << 5), (1 << 7), (1 << 6)};

void InitDO() {
  local_io_tx = GetLocalIoTx();
}

/**
 * @brief  Get number of digital output ports
 * @note   
 * @retval number of DOs ports
 */
uint32_t GetDONumber() {
  return DIGITAL_OUTPUT_PORTS;
}

/**
 * @brief Checks digital output bit and returns its state
 * @note   
 * @param  DONum: digital output port number
 * @retval true - high, false - low
 */
bool GetDO(uint32_t DONum) {
  if (DONum >= GetDONumber())
    return false;

  bool state = false;
  state = (local_io_tx->digital_output & DO_LUT[DONum]) == DO_LUT[DONum];
  return state;
}

/**
 * @brief  Sets digital output bit state
 * @note   
 * @param  state: true or false (high or low)
 * @param  DONum: digital output port number
 * @retval None
 */
void SetDO(bool state, uint32_t DONum) {
  if (DONum >= GetDONumber())
    return;

  if (state) {
    local_io_tx->digital_output |= DO_LUT[DONum];
  } else {
    local_io_tx->digital_output &= ~DO_LUT[DONum];
  }
}

/**
 * @brief  Toggle state of the digital output port of the UAC18
 * @note   
 * @param  DONum: digital output port number
 * @retval returns state of port after toggle
 */
void ToggleDO(uint32_t DONum) {
  if (DONum >= GetDONumber())
    return;

  local_io_tx->digital_output ^= DO_LUT[DONum];
}
