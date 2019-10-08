/*
 * Created on Mon Oct 07 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOG_NATIVE_H_
#define _ISMA_LOG_NATIVE_H_

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>

struct Library_isma_log_native_iSMA_Log_Log
{
    NANOCLR_NATIVE_DECLARE(AddLog___STATIC__VOID__iSMALogLogEntry);
    NANOCLR_NATIVE_DECLARE(GetLogs___STATIC__SZARRAY_iSMALogLogEntry);

    //--//

};

struct Library_isma_log_native_iSMA_Log_LogEntry
{
    static const int FIELD___timestamp = 1;
    static const int FIELD___logChannel = 2;
    static const int FIELD___text = 3;
    static const int FIELD___exception = 4;


    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_Log;

#endif  //_ISMA_LOG_NATIVE_H_
