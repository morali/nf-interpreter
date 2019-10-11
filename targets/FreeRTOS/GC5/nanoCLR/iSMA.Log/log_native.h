/*
 * Created on Fri Oct 11 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _LOG_NATIVE_H_
#define _LOG_NATIVE_H_

#include "isma_log_logLevel.h"

void addChannel(const char *channelName, logLevel_t level);
void addLog(const char *channel, logLevel_t level, const char *message);

#endif