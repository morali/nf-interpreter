//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>
#include <nanoHAL_Types.h>
#include <nanoCLR_Types.h>
#include <nanoHAL_Time.h>

#if defined(NXP_USE_RTC)
    #include "External_RTC.h"
#else
    #include "time.h"
    #include <sys/time.h>
#endif

// Returns the current date time from the RTC 
uint64_t  HAL_Time_CurrentDateTime(bool datePartOnly)
{
#ifdef NXP_USE_RTC
    
    SYSTEMTIME systemTime;
    struct tm *pRtcRealTime = RTC_ReadTime();

    systemTime.wMilliseconds = xTaskGetTickCount() % 1000;
    systemTime.wSecond       = pRtcRealTime->tm_sec;
    systemTime.wMinute       = pRtcRealTime->tm_min;
    systemTime.wHour         = pRtcRealTime->tm_hour;
    systemTime.wDay          = pRtcRealTime->tm_mday;
    systemTime.wMonth        = pRtcRealTime->tm_mon + 1;        // Add missing 1 month (for more information check: External_RTC.c)
    systemTime.wYear         = pRtcRealTime->tm_year + 1900;    // Add missing 1900 years (for more information check: External_RTC.c)

    // zero 'time' fields if date part only is required
    if(datePartOnly)
    {
        systemTime.wHour   = 0;
        systemTime.wMinute = 0;
        systemTime.wSecond = 0; 
    }
    
	return HAL_Time_ConvertFromSystemTime( &systemTime );
#else
    if (datePartOnly)
	{
		SYSTEMTIME st;
		HAL_Time_ToSystemTime(HAL_Time_CurrentTime(), &st);

		st.wHour = 0;
		st.wMinute = 0;
		st.wSecond = 0;
		st.wMilliseconds = 0;

		return HAL_Time_ConvertFromSystemTime(&st);
	}
	else
    {
        return HAL_Time_CurrentTime();
    }
#endif

}

void HAL_Time_SetUtcTime(uint64_t utcTime)
{
    SYSTEMTIME systemTime;

    HAL_Time_ToSystemTime(utcTime, &systemTime);

  #if defined(NXP_USE_RTC)

    
    // TODO


  #else
    // TODO FIXME
    // need to add implementation when RTC is not being used
    // can't mess with the systicks because the scheduling can fail
  #endif
}

bool HAL_Time_TimeSpanToStringEx( const int64_t& ticks, char*& buf, size_t& len )
{
    uint64_t ticksAbs;
    uint64_t rest;

    if(ticks < 0)
    {
        ticksAbs = -ticks;

        CLR_SafeSprintf( buf, len, "-" );
    }
    else
    {
        ticksAbs = ticks;
    }

    rest      = ticksAbs % ( 1000 * TIME_CONVERSION__TICKUNITS);
    ticksAbs  = ticksAbs / ( 1000 * TIME_CONVERSION__TICKUNITS);  // Convert to seconds.

    if(ticksAbs > TIME_CONVERSION__ONEDAY) // More than one day.
    {
        CLR_SafeSprintf( buf, len, "%d.", (int32_t)(ticksAbs / TIME_CONVERSION__ONEDAY) ); ticksAbs %= TIME_CONVERSION__ONEDAY;
    }

    CLR_SafeSprintf( buf, len, "%02d:", (int32_t)(ticksAbs / TIME_CONVERSION__ONEHOUR)  ); ticksAbs %= TIME_CONVERSION__ONEHOUR  ;
    CLR_SafeSprintf( buf, len, "%02d:", (int32_t)(ticksAbs / TIME_CONVERSION__ONEMINUTE)); ticksAbs %= TIME_CONVERSION__ONEMINUTE;
    CLR_SafeSprintf( buf, len, "%02d" , (int32_t)(ticksAbs / TIME_CONVERSION__ONESECOND)); ticksAbs %= TIME_CONVERSION__ONESECOND;

    ticksAbs = (uint32_t)rest;
    if(ticksAbs)
    {
        CLR_SafeSprintf( buf, len, ".%07d", (uint32_t)ticksAbs );
    }

    return len != 0;
}

bool DateTimeToString(const int64_t& time, char*& buf, size_t& len )
{
    SYSTEMTIME st;

    HAL_Time_ToSystemTime( time, &st );

    return CLR_SafeSprintf(buf, len, "%4d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
}

char* DateTimeToString(const int64_t& time)
{
    static char rgBuffer[128];
    char*  szBuffer =           rgBuffer;
    size_t iBuffer  = ARRAYSIZE(rgBuffer);

    DateTimeToString( time, szBuffer, iBuffer );

    return rgBuffer;
}

const char* HAL_Time_CurrentDateTimeToString()
{
    return DateTimeToString(HAL_Time_CurrentDateTime(false));
}

uint64_t CPU_MillisecondsToTicks(uint64_t ticks)
{
    return ticks * portTICK_PERIOD_MS;
}
