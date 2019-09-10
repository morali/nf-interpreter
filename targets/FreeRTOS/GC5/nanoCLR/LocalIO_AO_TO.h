
/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOCALIO_AO_TO_H_
#define _ISMA_LOCALIO_AO_TO_H_

#include <stdbool.h>
#include <stdint.h>

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

typedef struct _PWM_Config_t {
  uint16_t voltage;
  uint32_t duty_cycle;
  uint32_t pwm_count;
  AO_Mode_t mode;
  PWM_Freq_t frequency;
} AO_Config_t;

typedef enum {
  TO_PWM,
  TO_Digital
} TO_Mode_t;

typedef struct {
  uint32_t duty_cycle;
  uint32_t pwm_count;
  bool digital;
  TO_Mode_t mode;
  PWM_Freq_t frequency;
} TO_Config_t;

typedef struct _localIO_TO_AO {
  AO_Config_t AOconfig[ANALOG_OUTPUT_PORTS];
  TO_Config_t TOconfig[TRIAC_OUTPUT_PORTS];
} localIO_AO_TO_t;

#ifdef __cplusplus
extern "C" {
#endif

localIO_AO_TO_t *GetTOAOConfig();

uint32_t GetAONumber();
void SetAOMode(uint32_t id, AO_Mode_t pwm);
void SetAOFrequency(uint32_t id, PWM_Freq_t frequency);
void SetAODutyCycle(uint32_t id, uint32_t duty);
void SetAOVoltage(uint32_t id, uint16_t voltage);
void SetAODigital(uint32_t id, bool value);

uint32_t GetTONumber();
void SetTOMode(uint32_t id, TO_Mode_t mode);
void SetTOFrequency(uint32_t id, PWM_Freq_t frequency);
void SetTODutyCycle(uint32_t id, uint8_t duty);
void SetTODigital(uint32_t id, bool value);

#ifdef __cplusplus
}
#endif

#endif //_ISMA_LOCALIO_AO_H_
