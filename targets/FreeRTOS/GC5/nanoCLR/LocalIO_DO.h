
/*
 * Created on Mon Sep 09 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOCALIO_DO_H_
#define _ISMA_LOCALIO_DO_H_

#include <stdbool.h>
#include <stdint.h>

#define DIGITAL_OUTPUT_PORTS 5

void InitDO();

#ifdef __cplusplus
extern "C" {
#endif

uint32_t GetDONumber();
bool GetDO(uint32_t);
void SetDO(bool, uint32_t);
void ToggleDO(uint32_t);

#ifdef __cplusplus
}
#endif

#endif //_ISMA_LOCALIO_DO_H_
