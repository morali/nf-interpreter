/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */


#ifndef _MCP3421_H_
#define _MCP3421_H_

#include <stdint.h>
#include "fsl_common.h"

typedef enum {
    Gain_1 = 0,
    Gain_2 = 1,
    Gain_4 = 2,
    Gain_8 = 3,
    Gain_mask = 3
} PGAGain_t;

typedef enum {
    SampleRate_12bit = 0,
    SampleRate_14bit = 1,
    SampleRate_16bit = 2,
    // SampleRate_18bit = 3,
    SampleRate_mask = 3
} Samperate_t;


status_t MCP3421_SetGainSampleRate(PGAGain_t gain, Samperate_t samplerate);
status_t MCP3421_StartOneShot();
status_t MCP3421_ReadADC(int32_t * value);

#endif /* _MCP3421_H_ */