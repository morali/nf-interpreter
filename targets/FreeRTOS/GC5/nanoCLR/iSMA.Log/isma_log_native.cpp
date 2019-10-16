/*
 * Created on Mon Oct 07 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_log_native.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_log_native_iSMA_Log_FirmwareLog::AddLog___STATIC__VOID__iSMALogLogEntry,
    Library_isma_log_native_iSMA_Log_FirmwareLog::GetLogs___STATIC__SZARRAY_iSMALogLogEntry__U4__U4__BYREF_U4,
    Library_isma_log_native_iSMA_Log_FirmwareLog::SetLogBufferSize___STATIC__VOID__U2,
    Library_isma_log_native_iSMA_Log_FirmwareLog::GetLogBufferSize___STATIC__U2,
    Library_isma_log_native_iSMA_Log_FirmwareLog::SetChannelSetting___STATIC__VOID__STRING__iSMALogLogLevel,
    Library_isma_log_native_iSMA_Log_FirmwareLog::GetChannelSettings___STATIC__SZARRAY_iSMALogChannel,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_Log =
{
    "iSMA.Log", 
    0x651FEDE0,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    { 100, 0, 0, 0 }
};
