/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _LOCALIO_UI_H_
#define _LOCALIO_UI_H_

#include "MCP3421.h"

#define UINumber 4

typedef struct _UIConfig_t{
    Samperate_t samplerate;
    PGAGain_t gain;
    bool measureVoltage;
    bool measureResistance;
    uint8_t filter;
} UIConfig_t;

typedef struct _localIO_UI
{
    int16_t vRef;   //Reference Voltage
    UIConfig_t config[UINumber];
    int16_t voltage[UINumber];
    uint32_t resistance[UINumber];
    bool digital[UINumber];
} localIO_UI_t;

void vLocalIO_UI(void * argument);

/**
 * @brief  Returns number of UniversalInputs
 * @note   
 * @retval 
 */
uint32_t GetUINumber();

#endif /* _LOCALIO_UI_H_ */
