
/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOCALIO_AO_TO_H_
#define _ISMA_LOCALIO_AO_TO_H_

#include "isma_localio.h"

#define TRIAC_OUTPUT_PORTS 2
#define ANALOG_OUTPUT_PORTS 3

typedef struct _PWM_Config_t{
	bool PWM;
	uint16_t voltage;
	uint32_t duty_cycle;
	uint32_t frequency;
} PWM_Config_t;

typedef struct _localIO_AO {
	PWM_Config_t config[ANALOG_OUTPUT_PORTS];
	uint32_t pwm_count;
} localIO_AO_t;

typedef struct {
	uint32_t duty_cycle;
	uint32_t frequency;
} localIO_TO_t;

#ifdef __cplusplus
extern "C" {
#endif

void SetAOVoltage(uint32_t aoNo, uint16_t voltage);
void SetAOPWM(uint32_t aoNo, bool pwm);
void SetAOFrequency(uint32_t aoNo, uint32_t frequency);
void SetAODutyCycle(uint32_t aoNo, uint32_t duty);
uint32_t GetTONumber();
void SetTOFrequency(uint32_t id, uint32_t frequency);
void SetTODutyCycle(uint32_t id, uint8_t duty);

#ifdef __cplusplus
}
#endif

#endif  //_ISMA_LOCALIO_AO_H_
