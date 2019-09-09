/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "MCP4728.h"
#include "LocalIO_AO_TO.h"

localIO_AO_t s_local_ao;

void SetAOVoltage(uint32_t aoNo, uint16_t voltage)
{
    // status_t status;
    /* TODO: convert voltage from volts to 12bit value */
    MCP4728_ChannelSetValue(aoNo, voltage);
}

void SetAOPWM(uint32_t aoNo, bool pwm)
{
    if (aoNo > ANALOG_OUTPUT_PORTS)
        return;

    s_local_ao.AOconfig[aoNo].mode = pwm;
}

void SetAOFrequency(uint32_t aoNo, uint32_t frequency)
{
    if (aoNo > ANALOG_OUTPUT_PORTS)
        return;
    
    /* TODO: Add check for correct frequency */
    s_local_ao.AOconfig[aoNo].frequency = frequency;
}

void SetAODutyCycle(uint32_t aoNo, uint32_t duty)
{
    if (aoNo > ANALOG_OUTPUT_PORTS)
        return;

    /* TODO: Add check for correct duty cycle */
    s_local_ao.AOconfig[aoNo].duty_cycle = duty;
}


uint32_t GetTONumber()
{
    return TRIAC_OUTPUT_PORTS;
}

void SetTOFrequency(uint32_t id, uint32_t frequency) {
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