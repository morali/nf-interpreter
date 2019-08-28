#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ISMA_LOCALIO_AO_H_
#define _ISMA_LOCALIO_AO_H_

#include "isma_localio.h"

#define PWMNumber 3

typedef struct _PWM_Config_t{
    uint32_t duty_cycle;
    uint32_t frequency;
} PWM_Config_t;

typedef struct _localAO_UI {
  PWM_Config_t config[PWMNumber];
} localAO_UI_t;

#endif  //_ISMA_LOCALIO_AO_H_

#ifdef __cplusplus
}
#endif