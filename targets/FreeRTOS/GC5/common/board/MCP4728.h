 /*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */


#ifndef _MCP4728_H_
#define _MCP4728_H_

#include <stdint.h>
#include "fsl_common.h"
#include "i2c.h"

#define MCP4728_addr (0xC0 >> 1)

typedef enum _channel {
    channel_0 = 0,
    channel_4,
    channel_2,
    channel_3
} channel_t;


status_t MCP4728_ChannelSetValue(channel_t channel, uint16_t val);
status_t MCP4728_ChannelOff(channel_t channel);


#endif /* _MCP4728_H_ */