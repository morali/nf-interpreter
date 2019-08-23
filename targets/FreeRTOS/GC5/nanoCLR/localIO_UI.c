/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "localIO_UI.h"
#include "FreeRTOSCommonHooks.h"
#include "isma_localio.h"

typedef enum {
  SelectRefChannel,
  StartConvertVRef,
  ReadVRef,
  ReadResistance,
  ReadVoltage
} UIState_t;

#define MAX_RESISTANCE_PT1000 200000

static localIO_UI_t localIO_UI;

/**
 * @brief  Wait for conversion
 * @note   Because Conversion time depends resolution threre is need to wait different time
 * @param  samplerate: 
 * @retval None
 */
static void ConversionSleep(Samplerate_t samplerate) {
  switch (samplerate) {
  case SampleRate_12bit:
    FreeRTOSDelay(5);
    break;
  case SampleRate_14bit:
    FreeRTOSDelay(17);
    break;
  case SampleRate_16bit:
    FreeRTOSDelay(67);
    break;
  default:
    FreeRTOSDelay(267);
    break;
  }
}

static int32_t UniversalInputFilter(uint8_t tau, int64_t *memValue, int64_t *lastValue, uint64_t value) {

  //conditioning filter time
  if (tau > 60) {
    tau = 60;
  }

  //first measurement
  if (*memValue == 0) {
    *memValue = value * 128;
  }

  value *= 128;

  value = tau * (*memValue) + value;
  *memValue = value / (tau + 1);

  int64_t temp = *lastValue * 128 - *memValue;
  if (temp < 0) {
    temp *= -1;
  }

  if (temp > 112) {
    value = *memValue / 128;
  } else {
    value = *lastValue;
  }

  *lastValue = value;

  return value;
}

static bool configurationChanged(uint8_t configNo) {
  static uint8_t lastFilter[UINumber];

  if (lastFilter[configNo] != localIO_UI.config[configNo].filter) {
    lastFilter[configNo] = localIO_UI.config[configNo].filter;
    return true;
  }
  return false;
}

static bool sensorJustConnected(uint32_t last_res, uint32_t min, uint32_t max) {
  if (last_res <= min || last_res >= max) {
    return true;
  }

  return false;
}

static bool sensorNotConnected(int64_t value, uint32_t min, uint32_t max) {
  if (value <= min || value >= max) {
    return true;
  }

  return false;
}

static void resetFilter(int64_t *filter, int64_t *lastRes, int64_t value) {
  *filter = 0;
  *lastRes = value;
}

static void readUIResistance() {
  status_t status;
  uint8_t state;
  int32_t adc_value;
  int32_t adc_ref;

  static int64_t res_filter[UINumber] = {0};
  static int64_t last_res[UINumber] = {0};

  for (uint8_t i = 0; i < UINumber; i++) {
    if (localIO_UI.config[i].measureResistance) {
      state = 0;
      bool convertChannel = true;
      while (convertChannel) {
        switch (state) {
        case 0:
          SetUIChannelPullup(i, true);
          SetUIChannel(i);
          status = MCP3421_SetGainSampleRate(localIO_UI.config[i].gain, localIO_UI.config[i].samplerate);
          if (status == kStatus_Success) {
            FreeRTOSDelay(8);
            state = 1;
          }
          break;
        case 1:
          status = MCP3421_StartOneShot();
          if (status == kStatus_Success) {
            ConversionSleep(localIO_UI.config[i].samplerate);
            state = 2;
          }
          break;
        case 2:
          status = MCP3421_ReadADC(&adc_value);
          if (status == kStatus_Success) {
            //protection against small and negative values
            if (adc_value < 3) {
              adc_value = 0;
            }

            //resistance devider is 1/6
            adc_value = adc_value * 6;

            //correct PGA Gain
            switch (localIO_UI.config[i].gain) {
            case Gain_1:
              adc_value = (adc_value * 10024) / 10000;
              break;
            case Gain_2:
              adc_value = (adc_value * 10032) / 10000; //TODO check the correct adjustment for this gain
              break;
            case Gain_4:
              adc_value = (adc_value * 10039) / 10000;
              break;
            case Gain_8:
              adc_value = (adc_value * 10084) / 10000;
              break;
            }

            //correct value depening on ADC resolution
            switch (localIO_UI.config[i].samplerate) {
            case SampleRate_12bit:
              adc_value *= 16;
              break;
            case SampleRate_14bit:
              adc_value *= 4;
              break;
            case SampleRate_16bit:
              adc_value *= 1;
              break;
            default:
              break;
            }

            //conditioning vRef
            switch (localIO_UI.config[i].gain) {
            case Gain_1:
              adc_ref = localIO_UI.vRef * 16;
              break;
            case Gain_2:
              adc_ref = localIO_UI.vRef * 32;
              break;
            case Gain_4:
              adc_ref = localIO_UI.vRef * 64;
              break;
            case Gain_8:
              adc_ref = localIO_UI.vRef * 128;
              break;
            default:
              adc_ref = localIO_UI.vRef * 16;
            }

            if (adc_value >= adc_ref) {
              adc_value = adc_ref - 1;
            }

            //Calculate resistance
            adc_value = ((int64_t)adc_value * 200000) / (adc_ref - adc_value);
            if (adc_value >= 1200000) {
              adc_value = 1199999;
            }
            uint32_t adc_res = (uint64_t)((uint64_t)adc_value * 1200000) / (1200000 - adc_value);

            //if resistance is larger then 1M then trim because too big measurement error
            if (adc_res > 10000000) {
              adc_res = 10000000;
            }

            if (configurationChanged(i) || sensorNotConnected(adc_res, 0, MAX_RESISTANCE_PT1000) || sensorJustConnected(last_res[i], 0, MAX_RESISTANCE_PT1000)) {
              resetFilter(&res_filter[i], &last_res[i], adc_res);
            }
            adc_res = UniversalInputFilter(localIO_UI.config[i].filter, &res_filter[i], &last_res[i], adc_res);

            localIO_UI.resistance[i] = adc_res / 10;

            if (localIO_UI.resistance[i] < 5020) {
              localIO_UI.digital[i] = true;
            }
            if (localIO_UI.resistance[i] > 8090) {
              localIO_UI.digital[i] = false;
            }

            convertChannel = false;
          }
          break;

        default:
          break;
        }
      }

    } else {
      localIO_UI.resistance[i] = 0;
    }
  }
}

static void readUIVoltage() {
  status_t status;
  uint8_t state;
  int32_t adc_value;

  static int64_t vol_filter[UINumber] = {0};
  static int64_t last_vol[UINumber] = {0};

  for (uint8_t i = 0; i < UINumber; i++) {
    if (localIO_UI.config[i].measureVoltage) {
      state = 0;
      bool convertChannel = true;
      while (convertChannel) {
        switch (state) {
        case 0:
          SetUIChannelPullup(i, false);
          SetUIChannel(i);
          status = MCP3421_SetGainSampleRate(localIO_UI.config[i].gain, localIO_UI.config[i].samplerate);
          if (status == kStatus_Success) {
            FreeRTOSDelay(8);
            state = 1;
          }
          break;
        case 1:
          status = MCP3421_StartOneShot();
          if (status == kStatus_Success) {
            ConversionSleep(localIO_UI.config[i].samplerate);
            state = 2;
          }
          break;
        case 2:
          status = MCP3421_ReadADC(&adc_value);
          if (status == kStatus_Success) {
            //protection against small and negative values
            if (adc_value < 3) {
              adc_value = 0;
            }

            //resistance devider is 1/6
            adc_value = adc_value * 6;

            //correct PGA Gain
            switch (localIO_UI.config[i].gain) {
            case Gain_1:
              adc_value = (adc_value * 10024) / 10000;
              break;
            case Gain_2:
              adc_value = (adc_value * 10032) / 10000; //TODO check the correct adjustment for this gain
              break;
            case Gain_4:
              adc_value = (adc_value * 10039) / 10000;
              break;
            case Gain_8:
              adc_value = (adc_value * 10084) / 10000;
              break;
            }

            //correct value depening on ADC resolution
            switch (localIO_UI.config[i].samplerate) {
            case SampleRate_12bit:
              adc_value *= 16;
              break;
            case SampleRate_14bit:
              adc_value *= 4;
              break;
            case SampleRate_16bit:
              adc_value *= 1;
              break;
            default:
              break;
            }

            if (configurationChanged(i)) {
              resetFilter(&vol_filter[i], &last_vol[i], adc_value / 16);
            }
            adc_value = UniversalInputFilter(localIO_UI.config[i].filter, &vol_filter[i], &last_vol[i], adc_value / 16);

            localIO_UI.voltage[i] = adc_value;

            convertChannel = false;
          }
          break;

        default:
          break;
        }
      }

    } else {
      localIO_UI.voltage[i] = 0;
    }
  }
}

void vLocalIO_UI(void *argument) {
  (void)argument;

  UIState_t state = SelectRefChannel;
  status_t status;
  int32_t adc_value = 0;
  int32_t adc_vcc = 0;

  while (1) {
    switch (state) {
    case SelectRefChannel:
      //Measure Reference voltage
      SetUIChannel(VREF);
      status = MCP3421_SetGainSampleRate(Gain_1, SampleRate_12bit);
      if (status == kStatus_Success) {
        FreeRTOSDelay(8);
        state = StartConvertVRef;
      }
      break;
    case StartConvertVRef:
      status = MCP3421_StartOneShot();
      if (status == kStatus_Success) {
        ConversionSleep(SampleRate_12bit);
        state = ReadVRef;
      }
      break;
    case ReadVRef:
      status = MCP3421_ReadADC(&adc_value);
      if (status == kStatus_Success) {
        // VRef is divided by 2
        adc_value *= 2;

        // Correct PGA Gain
        adc_value = (adc_value * 10024) / 10000;

        // Filter VREF
        if (adc_vcc == 0) {
          adc_vcc = adc_value;
        }

        adc_value = (8 - 1) * adc_vcc + adc_value;
        adc_value = adc_value / 8;
        adc_vcc = adc_value;

        localIO_UI.vRef = adc_value;

        state = ReadResistance;
      }
      break;
    case ReadResistance:
      readUIResistance();
      state = ReadVoltage;
      break;

    case ReadVoltage:
      readUIVoltage();
      state = SelectRefChannel;
      break;

    default:
      break;
    }
  }
}

uint32_t GetUINumber() {
  return UINumber;
}

void SetUIResolution(uint32_t no, Samplerate_t resolution) {
  localIO_UI.config[no].samplerate = resolution;
}

void SetUIGain(uint32_t no, PGAGain_t gain) {
  localIO_UI.config[no].gain = gain;
}

void SetUIFilter(uint32_t no, uint8_t filterTime) {
  localIO_UI.config[no].filter = filterTime;
}

void SetUIMeasureVoltage(uint32_t no, bool measureVoltage) {
  localIO_UI.config[no].measureVoltage = measureVoltage;
}

void SetUIMeasureResistance(uint32_t no, bool measureResistance) {
  localIO_UI.config[no].measureResistance = measureResistance;
}

int16_t GetUIVoltage(uint32_t no) {
  return localIO_UI.voltage[no];
}

uint32_t GetUIResistance(uint32_t no) {
  return localIO_UI.resistance[no];
}

bool GetUIDigital(uint32_t no) {
  return localIO_UI.digital[no];
}