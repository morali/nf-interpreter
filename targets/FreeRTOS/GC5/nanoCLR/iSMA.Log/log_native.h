/*
 * Created on Fri Oct 11 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _LOG_NATIVE_H_
#define _LOG_NATIVE_H_

/////////////////////////////////////////////////////////////////////////////////////////////////////
// !!! KEEP IN SYNC WITH iSMA.Log.Log.LogLevel (in managed code) !!! //
/////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum LogLevel
{
    Spam = 0,
    Troubleshot,
    Important,
    Error,
    Critical,
    Undefined
} logLevel_t;

void addChannel(const char *channelName, logLevel_t level);
bool addLog(const char *channel, logLevel_t level, const char *message);

#endif