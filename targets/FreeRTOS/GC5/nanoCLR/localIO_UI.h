/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _LOCALIO_UI_H_
#define _LOCALIO_UI_H_

#define MAX_RESISTANCE_PT1000 200000
#define UINumber 4U

#include "MCP3421.h"
#include "FreeRTOSCommonHooks.h"
#include "isma_localio.h"

typedef enum {
  SelectRefChannel,
  StartConvertVRef,
  ReadVRef,
  ReadResistance,
  ReadVoltage
} UIState_t;

typedef enum {
    UI1,
    UI2,
    UI3,
    UI4,
    UI5,
    UI6,
    UI7,
    UI8,
    VREF
} UIChannel_t;

typedef struct _UIConfig_t{
    Samplerate_t samplerate;
    PGAGain_t gain;
    bool measureVoltage;
    bool measureResistance;
    uint8_t filter;
} UIConfig_t;

typedef struct _localIO_UI {
  int16_t vRef; //Reference Voltage
  UIConfig_t config[UINumber];
  int16_t voltage[UINumber];
  uint32_t resistance[UINumber];
  bool digital[UINumber];
} localIO_UI_t;

void vLocalIO_UI(void *argument);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Returns number of UniversalInputs
 * @note   
 * @retval 
 */
uint32_t GetUINumber();

/**
 * @brief  Set Universal Input Resolution / Samplerate
 * @note   
 * @param  no: Input number
 * @param  resolution: Desired Resolution / Samplerate
 * @retval None
 */
void SetUIResolution(uint32_t no, Samplerate_t resolution);

/**
 * @brief  Set Universal Input ADC PGA Gain
 * @note   
 * @param  no: Input number
 * @param  gain: Desired ADC PGA Gain
 * @retval None
 */
void SetUIGain(uint32_t no, PGAGain_t gain);

/**
 * @brief  Set Universal Input Filter time
 * @note   
 * @param  no: Input number
 * @param  filterTime: Desired Filter time
 * @retval None
 */
void SetUIFilter(uint32_t no, uint8_t filterTime);

/**
 * @brief  Enable/Disable Universal Input Voltage Measurement
 * @note   
 * @param  no: Input number
 * @param  measureVoltage: if true then measurement is enabled
 * @retval None
 */
void SetUIMeasureVoltage(uint32_t no, bool measureVoltage);

/**
 * @brief  Enable/Disable Universal Input Resistance Measurement
 * @note   
 * @param  no: Input number
 * @param  measureResistance: if true then measurement is enabled
 * @retval None
 */
void SetUIMeasureResistance(uint32_t no, bool measureResistance);

/**
 * @brief  Return last measured voltage
 * @note   
 * @param  no: input number
 * @retval 
 */
int16_t GetUIVoltage(uint32_t no);

/**
 * @brief  Return last measured resistance
 * @note   
 * @param  no: input number
 * @retval 
 */
uint32_t GetUIResistance(uint32_t no);

/**
 * @brief  Return last measured digital state
 * @note   
 * @param  no: input number
 * @retval 
 */
bool GetUIDigital(uint32_t no);

#ifdef __cplusplus
}
#endif
/**
 * @brief  Switches the analog multiplexer to the appropriate channel
 * @note   
 * @param  channel: UIChannel_t
 * @retval None
 */
void SetUIChannel(UIChannel_t channel);

/**
 * @brief  Enable or disable pullup for resistance measurement
 * @note   
 * @param  channel: channel number
 * @param  enable: if true then pullup is enabled
 * @retval None
 */
void SetUIChannelPullup(UIChannel_t channel, bool enable);

void vLocalIO_UI(void * argument);

#endif /* _LOCALIO_UI_H_ */
