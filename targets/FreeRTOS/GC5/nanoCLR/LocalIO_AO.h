#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ISMA_LOCALIO_AO_H_
#define _ISMA_LOCALIO_AO_H_

#include "isma_localio.h"
#include "MCP4728.h"

#define ANALOG_OUTPUT_PORTS 5
#define PWMNumber 3

typedef struct _PWM_Config_t{
	bool PWM;
	uint16_t voltage;
	uint32_t duty_cycle;
	uint32_t frequency;
} PWM_Config_t;

typedef struct _localIO_AO {
	PWM_Config_t config[PWMNumber];
	uint32_t pwm_count;
} localIO_AO_t;

uint32_t GetAONumber(void);

void SetAOVoltage(uint32_t aoNo, uint16_t voltage);
void SetAOPWM(uint32_t aoNo, bool pwm);
void SetAOFrequency(uint32_t aoNo, uint32_t frequency);
void SetAODutyCycle(uint32_t aoNo, uint32_t duty);

#endif  //_ISMA_LOCALIO_AO_H_

#ifdef __cplusplus
}
#endif