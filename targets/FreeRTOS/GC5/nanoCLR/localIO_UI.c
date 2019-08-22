/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "MCP3421.h"
#include "isma_localio.h"

typedef enum {
  SelectRefChannel,
  StartConvertRef,
  ReadRef
} UIState_t;

void vLocalIO_UI(void *argument) {
  (void)argument;

  UIState_t state = SelectRefChannel;
  status_t status;
  int32_t adc_value = 0;

  while (1) {
    switch (state) {
    case SelectRefChannel:
      //Measure Reference voltage
      SetUIChannel(VREF);
      status = MCP3421_SetGainSampleRate(Gain_1, SampleRate_12bit);
      if (status == kStatus_Success) {
        state = StartConvertRef;
      }
      break;
    case StartConvertRef:
      status = MCP3421_StartOneShot();
      if (status == kStatus_Success) {
        state = ReadRef;
      }
      break;
    case ReadRef:
      status = MCP3421_ReadADC(&adc_value);
      if (status == kStatus_Success) {
        // VRef is divided by 2
        adc_value *= 2;

        // Correct PGA Gain
        adc_value = (adc_value * 10024) / 10000;
        state = StartConvertRef;
      }
      break;

    default:
      break;
    }
  }
}