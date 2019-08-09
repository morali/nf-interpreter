/*
 * External_RTC.c
 *
 *  Created on: 01.08.2019
 *      Author: Jakub Standarski
 */

#include "External_RTC.h"

///////////////////////////////////////////
//        RTC private declarations       //
///////////////////////////////////////////
static struct tm rtcRealTime;




/************************************************************************************************************/
/*                                                                                                          */
/*                                            RTC DEFINES                                                   */
/*                                                                                                          */
/************************************************************************************************************/

/* RTC generic defines */
#define RTC_I2C3_ADDRESS           (uint16_t)0x51
#define RTC_DATA_SIZE              (size_t)0x07
#define RTC_REGISTER_SIZE          (size_t)0x01


/* RTC registers addresses */
#define RTC_VL_SECONDS_REGISTER    (uint8_t)0x02


/* RTC registers positions */
enum time_registers_positions_t
{
    SECONDS,
    MINUTES,
    HOURS,
    DAYS,
    WEEKDAYS,
    MONTHS,
    YEARS
};




/************************************************************************************************************/
/*                                                                                                          */
/*                                     ADDITIONAL FUNCTIONS DEFINITIONS                                      */
/*                                                                                                          */
/************************************************************************************************************/

static void MaskRtcData(uint8_t *rtcData)
{
    rtcData[SECONDS]    &= 0x7F;
    rtcData[MINUTES]    &= 0x7F;
    rtcData[HOURS]      &= 0x3F;
    rtcData[DAYS]       &= 0x3F;
    rtcData[WEEKDAYS]   &= 0x07;
    rtcData[MONTHS]     &= 0x1F;
    rtcData[YEARS]      &= 0xFF;
}




/************************************************************************************************************/
/*                                                                                                          */
/*                                     RTC READ TIME FUNCTIONS DEFINITIONS                                  */
/*                                                                                                          */
/************************************************************************************************************/

static void ConvertBcdIntoRealTime(uint8_t *rtcReadData)
{
    for(uint8_t rtcRegister = SECONDS; rtcRegister <= YEARS; rtcRegister++)
    {
        rtcReadData[rtcRegister] = (rtcReadData[rtcRegister] & 0xF) + ((rtcReadData[rtcRegister] >> 4) & 0xF) * 10;   
    }
}



static status_t ConversionErrorCheck(uint8_t *rtcRealReadTime)
{
    if(rtcRealReadTime[SECONDS] > 59 || rtcRealReadTime[MINUTES] > 59 || rtcRealReadTime[HOURS] > 23 || rtcRealReadTime[DAYS] > 31 || rtcRealReadTime[WEEKDAYS] > 6 || rtcRealReadTime[MONTHS] > 12 || rtcRealReadTime[YEARS] > 99) 
    {
        return kStatus_Fail;
    }
    else
    {
        return kStatus_Success;
    }  
}



static status_t ParseBcdIntoRealTime(uint8_t *rtcReadData)
{
    MaskRtcData(rtcReadData);

    ConvertBcdIntoRealTime(rtcReadData);

    status_t conversionStatus = ConversionErrorCheck(rtcReadData);
    return conversionStatus;
} 



static void SaveRealTimeIntoStructure(struct tm *rtcTime, uint8_t *rtcRealReadTime)
{
    rtcTime->tm_sec  = rtcRealReadTime[SECONDS];
    rtcTime->tm_min  = rtcRealReadTime[MINUTES];
    rtcTime->tm_hour = rtcRealReadTime[HOURS];
    rtcTime->tm_mday = rtcRealReadTime[DAYS];
    rtcTime->tm_wday = rtcRealReadTime[WEEKDAYS];
    rtcTime->tm_mon  = rtcRealReadTime[MONTHS] - 1;      // Substract 1 to ensure proper functionality of mktime() - it counts months from 0 to 11
    rtcTime->tm_year = rtcRealReadTime[YEARS] + 100;     // Add 100 to ensure proper functionality of mktime() - it counts year since 1900
}




/************************************************************************************************************/
/*                                                                                                          */
/*                                     RTC SET TIME FUNCTIONS DEFINITIONS                                   */
/*                                                                                                          */
/************************************************************************************************************/

static void ConvertRealTimeIntoBcd(uint8_t *writeTime, struct tm *utcTime)
{
    writeTime[SECONDS]  = ((utcTime->tm_sec  / 10) << 4) | (utcTime->tm_sec  % 10);
    writeTime[MINUTES]  = ((utcTime->tm_min  / 10) << 4) | (utcTime->tm_min  % 10);
    writeTime[HOURS]    = ((utcTime->tm_hour / 10) << 4) | (utcTime->tm_hour % 10);
    writeTime[DAYS]     = ((utcTime->tm_mday / 10) << 4) | (utcTime->tm_mday % 10);
    writeTime[WEEKDAYS] = (utcTime->tm_wday);
    writeTime[MONTHS]   = (((utcTime->tm_mon  + 1)   / 10) << 4) | ((utcTime->tm_mon  + 1)   % 10);
    writeTime[YEARS]    = (((utcTime->tm_year - 100) / 10) << 4) | ((utcTime->tm_year - 100) % 10);
}




/************************************************************************************************************/
/*                                                                                                          */
/*                                       RTC PUBLIC APIs DEFINITIONS                                        */
/*                                                                                                          */
/************************************************************************************************************/

struct tm *RTC_ReadTime()
{
    return &rtcRealTime;
}



void RTC_SetTime(struct tm *rtcSetTime)
{
    rtcRealTime = *rtcSetTime; // Copy set time into real time structure to allow proper immediate read procedure

    lpi2c_master_transfer_t rtcWriteTransfer;

    uint8_t writeTime[RTC_DATA_SIZE];
    ConvertRealTimeIntoBcd(writeTime, rtcSetTime);

    I2C_MasterStructureInit(&rtcWriteTransfer, RTC_I2C3_ADDRESS, RTC_VL_SECONDS_REGISTER, RTC_REGISTER_SIZE, DEFAULT_FLAG);
    I2C_SetBufferAndDirection(&rtcWriteTransfer, writeTime, RTC_DATA_SIZE, WRITE);
    LPI2C_RTOS_Transfer(&i2c3.masterRtosHandle, &rtcWriteTransfer);
}




/************************************************************************************************************/
/*                                                                                                          */
/*                                           FreeRTOS TASK                                                  */
/*                                                                                                          */
/************************************************************************************************************/

void vRtcThread(void *pvParameters)
{   
    (void)pvParameters;
    lpi2c_master_transfer_t rtcReadTransfer;
    uint8_t rtcReadTime[RTC_DATA_SIZE];
    struct tm rtcTempTime;
        
    I2C_MasterStructureInit(&rtcReadTransfer, RTC_I2C3_ADDRESS, RTC_VL_SECONDS_REGISTER, RTC_REGISTER_SIZE, DEFAULT_FLAG);
    I2C_SetBufferAndDirection(&rtcReadTransfer, rtcReadTime, RTC_DATA_SIZE, READ);

    for(;;)
    {
        LPI2C_RTOS_Transfer(&i2c3.masterRtosHandle, &rtcReadTransfer);
      
        if (ParseBcdIntoRealTime(rtcReadTime) == kStatus_Success)
        {
            SaveRealTimeIntoStructure(&rtcTempTime, rtcReadTime);
            time_t checkTime1 = mktime(&rtcTempTime);

            LPI2C_RTOS_Transfer(&i2c3.masterRtosHandle, &rtcReadTransfer);
            
            if (ParseBcdIntoRealTime(rtcReadTime) == kStatus_Success)
            {
                SaveRealTimeIntoStructure(&rtcTempTime, rtcReadTime);
                time_t checkTime2 = mktime(&rtcTempTime);

                if ((checkTime2 - checkTime1 == 1) || (checkTime2 - checkTime1) == 0)
                {
                    // RTC time read correctly
                    rtcRealTime = rtcTempTime;                    
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(400));
    }
    vTaskDelete(NULL);
}