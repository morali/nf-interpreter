
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

typedef enum {
	AO_Voltage,
	AO_PWM,
	AO_Digital
} AO_Mode_t;

typedef enum {
	PWM_1,
	PWM_10,
	PWM_100,
	PWM_01,
	PWM_001,
} PWM_Freq_t;

typedef struct _PWM_Config_t{
	uint16_t voltage;
	uint32_t duty_cycle;
	uint32_t pwm_count;
	AO_Mode_t mode;
	PWM_Freq_t frequency;
} AO_Config_t;

typedef struct {
	uint32_t duty_cycle;
	uint32_t pwm_count;
	PWM_Freq_t frequency;
} TO_Config_t;

typedef struct _localIO_AO {
	AO_Config_t AOconfig[ANALOG_OUTPUT_PORTS];
	TO_Config_t TOconfig[TRIAC_OUTPUT_PORTS];
} localIO_AO_t;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t GetAONumber();
void SetAOVoltage(uint32_t aoNo, uint16_t voltage);
void SetAOMode(uint32_t aoNo, AO_Mode_t pwm);
void SetAOFrequency(uint32_t aoNo, PWM_Freq_t frequency);
void SetAODutyCycle(uint32_t aoNo, uint32_t duty);
void SetAODigital(uint32_t aoNo, bool value);

uint32_t GetTONumber();
void SetTOFrequency(uint32_t id, PWM_Freq_t frequency);
void SetTODutyCycle(uint32_t id, uint8_t duty);

#ifdef __cplusplus
}
#endif

#endif  //_ISMA_LOCALIO_AO_H_
