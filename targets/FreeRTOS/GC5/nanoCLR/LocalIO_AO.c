#ifdef __cplusplus
extern "C" {
#endif

#include "LocalIO_AO.h"
#include "MCP4728.h"

localIO_AO_t local_ao;

void SetAOVoltage(uint32_t aoNo, uint16_t voltage)
{
    // status_t status;
    /* TODO: convert voltage from volts to 12bit value */
    MCP4728_ChannelSetValue(aoNo, voltage);
    // configASSERT(status);
}

void SetAOPWM(uint32_t aoNo, bool pwm)
{
    if (aoNo > ANALOG_OUTPUT_PORTS)
        return;

    local_ao.config[aoNo].PWM = pwm;
}

void SetAOFrequency(uint32_t aoNo, uint32_t frequency)
{
    if (aoNo > ANALOG_OUTPUT_PORTS)
        return;
    
    /* TODO: Add check for correct frequency */
    local_ao.config[aoNo].frequency = frequency;
}

void SetAODutyCycle(uint32_t aoNo, uint32_t duty)
{
    if (aoNo > ANALOG_OUTPUT_PORTS)
        return;

    /* TODO: Add check for correct duty cycle */
    local_ao.config[aoNo].duty_cycle = duty;
}

#ifdef __cplusplus
}
#endif