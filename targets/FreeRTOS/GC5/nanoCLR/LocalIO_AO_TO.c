/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "LocalIO_AO_TO.h"
#include "MCP4728.h"

localIO_AO_t s_local_ao;

uint32_t GetAONumber() {
  return ANALOG_OUTPUT_PORTS;
}

static const uint8_t AO_LUT[] = {3, 2, 0};

void SetAOVoltage(uint32_t aoNo, uint16_t voltage) {
  if (aoNo > ANALOG_OUTPUT_PORTS)
    return;

  s_local_ao.AOconfig[aoNo].voltage = voltage;
  uint16_t value = (voltage * 2048) / 5145;
  if (value > 4095) {
    value = 4095;
  }
  MCP4728_ChannelSetValue(AO_LUT[aoNo], value);
}

void SetAOPWM(uint32_t aoNo, bool pwm) {
  if (aoNo > ANALOG_OUTPUT_PORTS)
    return;

  s_local_ao.AOconfig[aoNo].mode = pwm;

  if (pwm) {
    MCP4728_ChannelOff(AO_LUT[aoNo]);
  } else {
    MCP4728_ChannelSetValue(AO_LUT[aoNo], 0);
  }
}

void SetAOFrequency(uint32_t aoNo, PWM_Freq_t frequency) {
  if (aoNo > GetAONumber())
    return;

  s_local_ao.AOconfig[aoNo].frequency = frequency;
}

void SetAODutyCycle(uint32_t aoNo, uint32_t duty) {
  if (aoNo > GetAONumber())
    return;

  s_local_ao.AOconfig[aoNo].duty_cycle = duty;
}

uint32_t GetTONumber() {
  return TRIAC_OUTPUT_PORTS;
}

void SetTOFrequency(uint32_t id, PWM_Freq_t frequency) {
  if (id > GetTONumber()) {
    return;
  }

  /* TODO: Add check for correct frequency */
  s_local_ao.AOconfig[id].frequency = frequency;
}

void SetTODutyCycle(uint32_t id, uint8_t duty) {
  if (id > GetTONumber()) {
    return;
  }

  if (duty > 100) {
    duty = 100;
  }

  s_local_ao.AOconfig[id].duty_cycle = duty;
}