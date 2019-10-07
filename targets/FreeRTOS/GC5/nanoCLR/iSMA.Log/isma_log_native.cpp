/*
 * Created on Mon Oct 07 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_log_native.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    Library_isma_log_native_iSMA_Log_Log::AddLog___STATIC__VOID__iSMALogLogEntry,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_Log =
{
    "iSMA.Log", 
    0xAF84C2D0,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    { 100, 0, 0, 0 }
};
