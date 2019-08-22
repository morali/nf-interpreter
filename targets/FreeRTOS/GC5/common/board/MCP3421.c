/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "MCP3421.h"
#include "i2c.h"

static uint8_t MCP3421_cfg = 0;

#define MCP3421_addr 0xD0

#define Gain_offset 0
#define SampleRate_offset 2
#define ConversionMode_offset 4
#define ReadyBit_offset 7

status_t MCP3421_SetGainSampleRate(PGAGain_t gain, Samperate_t samplerate)
{
    //set gain
    MCP3421_cfg &= ~(Gain_mask << Gain_offset);
    MCP3421_cfg |= (gain << Gain_offset);

    //set samplerate
    MCP3421_cfg &= ~(SampleRate_mask << SampleRate_offset);
    MCP3421_cfg |= (samplerate << SampleRate_offset);

    return I2CTransfer(&i2c2, MCP3421_addr, kLPI2C_Write, 0, 0, &MCP3421_cfg, 1);
}

status_t MCP3421_StartOneShot()
{
    uint8_t out = MCP3421_cfg | (1 << ReadyBit_offset); //set status bit to start one shot conversion
    return I2CTransfer(&i2c2, MCP3421_addr, kLPI2C_Write, 0, 0, &out, 1);
}

status_t MCP3421_ReadADC(int32_t * value)
{
    uint8_t in[3];
    status_t status = I2CTransfer(&i2c2, MCP3421_addr, kLPI2C_Read, 0, 0, in, 3);

    if (status == kStatus_Success) {
        if (in[2] & (1 << ReadyBit_offset))
        {
            //conversion not ready
            return kStatus_LPI2C_Busy;
        }
        int16_t val = (in[0] << 8) | in[1];   
        *value = val;      
    }

    return status;
}