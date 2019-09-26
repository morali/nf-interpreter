/*
 * Created on Thu Sep 26 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "External_RTC.h"
#include "FreeRTOSCommonHooks.h"
#include "i2c.h"

static struct tm rtcRealTime;

/* RTC generic defines */
#define RTC_ADDRESS 0x51
#define RTC_DATA_SIZE 0x07
#define RTC_REGISTER_SIZE 0x01

/* RTC registers addresses */
#define RTC_VL_SECONDS_REGISTER (uint8_t)0x02

/* RTC registers positions */
enum time_registers_positions_t {
  SECONDS,
  MINUTES,
  HOURS,
  DAYS,
  WEEKDAYS,
  MONTHS,
  YEARS
};

/**
 * @brief  Apply mask to all bytes to remove unnecessary bytes 
 * @note   
 * @param  *rtcData: 
 * @retval None
 */
static void MaskRtcData(uint8_t *rtcData) {
  static const uint8_t masks[] = {0x7F, 0x7F, 0x3F, 0x3F, 0x07, 0x1F, 0xFF};

  for (int i = SECONDS; i < YEARS; i++) {
    rtcData[i] &= masks[i];
  }
}

static void ConvertBcdIntoRealTime(uint8_t *rtcReadData) {
  for (uint8_t rtcRegister = SECONDS; rtcRegister <= YEARS; rtcRegister++) {
    rtcReadData[rtcRegister] = (rtcReadData[rtcRegister] & 0xF) + ((rtcReadData[rtcRegister] >> 4) & 0xF) * 10;
  }
}

static status_t ConversionErrorCheck(uint8_t *rtcRealReadTime) {
  if (rtcRealReadTime[SECONDS] > 59 || rtcRealReadTime[MINUTES] > 59 || rtcRealReadTime[HOURS] > 23 || rtcRealReadTime[DAYS] > 31 || rtcRealReadTime[WEEKDAYS] > 6 || rtcRealReadTime[MONTHS] > 12 || rtcRealReadTime[YEARS] > 99) {
    return kStatus_Fail;
  } else {
    return kStatus_Success;
  }
}

static status_t ParseBcdIntoRealTime(uint8_t *rtcReadData) {
  MaskRtcData(rtcReadData);

  ConvertBcdIntoRealTime(rtcReadData);

  status_t conversionStatus = ConversionErrorCheck(rtcReadData);
  return conversionStatus;
}

static void SaveRealTimeIntoStructure(struct tm *rtcTime, uint8_t *rtcRealReadTime) {
  rtcTime->tm_sec = rtcRealReadTime[SECONDS];
  rtcTime->tm_min = rtcRealReadTime[MINUTES];
  rtcTime->tm_hour = rtcRealReadTime[HOURS];
  rtcTime->tm_mday = rtcRealReadTime[DAYS];
  rtcTime->tm_wday = rtcRealReadTime[WEEKDAYS];
  rtcTime->tm_mon = rtcRealReadTime[MONTHS] - 1;   // Substract 1 to ensure proper functionality of mktime() - it counts months from 0 to 11
  rtcTime->tm_year = rtcRealReadTime[YEARS] + 100; // Add 100 to ensure proper functionality of mktime() - it counts year since 1900
}

static void ConvertRealTimeIntoBcd(uint8_t *writeTime, struct tm *utcTime) {
  writeTime[SECONDS] = ((utcTime->tm_sec / 10) << 4) | (utcTime->tm_sec % 10);
  writeTime[MINUTES] = ((utcTime->tm_min / 10) << 4) | (utcTime->tm_min % 10);
  writeTime[HOURS] = ((utcTime->tm_hour / 10) << 4) | (utcTime->tm_hour % 10);
  writeTime[DAYS] = ((utcTime->tm_mday / 10) << 4) | (utcTime->tm_mday % 10);
  writeTime[WEEKDAYS] = (utcTime->tm_wday);
  writeTime[MONTHS] = (((utcTime->tm_mon + 1) / 10) << 4) | ((utcTime->tm_mon + 1) % 10);
  writeTime[YEARS] = (((utcTime->tm_year - 100) / 10) << 4) | ((utcTime->tm_year - 100) % 10);
}

struct tm *RTC_ReadTime() {
  return &rtcRealTime;
}

void RTC_SetTime(struct tm *rtcSetTime) {
  rtcRealTime = *rtcSetTime; // Copy set time into real time structure to allow proper immediate read procedure

  uint8_t writeTime[RTC_DATA_SIZE];
  ConvertRealTimeIntoBcd(writeTime, rtcSetTime);

  I2CTransfer(GetI2C3_Handle(), RTC_ADDRESS, kLPI2C_Write, RTC_VL_SECONDS_REGISTER, RTC_REGISTER_SIZE, writeTime, RTC_DATA_SIZE);
}

void vRtcThread(void *pvParameters) {
  (void)pvParameters;
  uint8_t rtcReadTime[RTC_DATA_SIZE];
  struct tm rtcTempTime;
  status_t status;

  lpi2c_rtos_handle_t *i2c3_handle = GetI2C3_Handle();
  for (;;) {
    FreeRTOSDelay(400);

    // Read datetime for the first time. If not successful then start from beginning
    status = I2CTransfer(i2c3_handle, RTC_ADDRESS, kLPI2C_Read, RTC_VL_SECONDS_REGISTER, RTC_REGISTER_SIZE, rtcReadTime, RTC_DATA_SIZE);
    if (status != kStatus_Success) {
      continue;
    }

    // Try parse datetime. If not successful then start from beginning
    status = ParseBcdIntoRealTime(rtcReadTime);
    if (status != kStatus_Success) {
      continue;
    }

    SaveRealTimeIntoStructure(&rtcTempTime, rtcReadTime);
    time_t checkTime1 = mktime(&rtcTempTime);

    // Read datetime for the second time. If not successful then start from beginning
    status = I2CTransfer(i2c3_handle, RTC_ADDRESS, kLPI2C_Read, RTC_VL_SECONDS_REGISTER, RTC_REGISTER_SIZE, rtcReadTime, RTC_DATA_SIZE);
    if (status != kStatus_Success) {
      continue;
    }

    // Try parse datetime. If not successful then start from beginning
    status = ParseBcdIntoRealTime(rtcReadTime);
    if (status != kStatus_Success) {
      continue;
    }

    SaveRealTimeIntoStructure(&rtcTempTime, rtcReadTime);
    time_t checkTime2 = mktime(&rtcTempTime);

    // If difference between first and second time is 0 or 1 second then can assume the readed time is correct and save to rtcRealTime
    if ((checkTime2 - checkTime1 == 1) || (checkTime2 - checkTime1) == 0) {
      // RTC time read correctly
      rtcRealTime = rtcTempTime;
    }
  }
}