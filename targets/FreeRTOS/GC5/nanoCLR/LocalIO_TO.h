/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOCALIO_AO_H_
#define _ISMA_LOCALIO_AO_H_


#ifdef __cplusplus
extern "C" {
#endif

void SetTOFrequency(uint32_t id, uint32_t frequency);
void SetTODutyCycle(uint32_t id, uint8_t duty);

#ifdef __cplusplus
}
#endif

#endif  //_ISMA_LOCALIO_AO_H_