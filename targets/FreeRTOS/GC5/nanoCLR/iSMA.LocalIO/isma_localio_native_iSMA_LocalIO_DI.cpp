/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */


#include "isma_localio_native.h"
#include "LocalIO_DI.h"


HRESULT Library_isma_localio_native_iSMA_LocalIO_DI::DIGetNative___BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t id = pThis[FIELD___id].NumericByRefConst().u4;
        bool diState = GetDIState(id);
        
        stack.SetResult_Boolean(diState);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_DI::DIGetCounterNative___U4( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t id = pThis[FIELD___id].NumericByRefConst().u4;
        uint32_t value = GetDICounter(id);
        
        stack.SetResult_U4(value);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_DI::DISetCounterNative___VOID__U4( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t id = pThis[FIELD___id].NumericByRefConst().u4;
        uint32_t value = stack.Arg1().NumericByRef().s4;
        
        SetDICounter(id, value);
    }
    NANOCLR_NOCLEANUP();
}