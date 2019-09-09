/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "MCP4728.h"

channel_t s_channel;

/**
 * @brief  Sets desired value on MCP4728 ADC chip
 * @note   
 * @param  s_channel: channel of the MPC4728
 * @param  val: 12bit value
 * @retval if transmission was a sucess
 */
status_t MCP4728_ChannelSetValue(channel_t s_channel, uint16_t val)
{
    uint8_t command[3];
    status_t status;

    command[0] = (2<<5) | ((s_channel & 0x03) << 1); 	//multi write command, kanal, udac = 0
	command[1] = (1<<7) | (0<<4) | (val >> 8); 	//VREF i  najstarsze bity
	command[2] = val; 
    
    status = I2CTransfer(&i2c2, MCP4728_addr, kLPI2C_Write, 0, 0, command, 3);
    return status;
}

status_t MCP4728_ChannelGetValue(channel_t s_channel)
{
    /* STUB */
    (void) s_channel;
    return 1;
}

/**
 * @brief  Disables channel of the MCP4728 ADC chip
 * @note   
 * @param  s_channel: channel to disable
 * @retval if disabling was a success
 */
status_t MCP4728_ChannelOff(channel_t s_channel)
{
    uint8_t command[3];
    status_t status = -1;

    command[0] = (2 << 5) | ((s_channel & 0x03) << 1); /* multi write command with udac = 0 */
    command[1] = (3 << 5);  /* powerdown (500k do masy) */
    command[3] = 0;

    status = I2CTransfer(&i2c2, MCP4728_addr, kLPI2C_Write, 0, 0, command, 3);
    return status;
}