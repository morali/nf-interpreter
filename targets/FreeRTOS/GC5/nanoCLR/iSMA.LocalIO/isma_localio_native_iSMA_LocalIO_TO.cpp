/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_localio_native.h"
#include "LocalIO_TO.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_TO::SetConfigNative___VOID( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___id].NumericByRefConst().u4;
        uint32_t frequency = pThis[FIELD___frequency].NumericByRefConst().u1;

        SetTOFrequency(aoNo, frequency);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_TO::SetDutyNative___VOID__U1( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___id].NumericByRefConst().u4;
        uint8_t duty = stack.Arg1().NumericByRef().u1;

        SetTODutyCycle(aoNo, duty);
    }
    NANOCLR_NOCLEANUP();
}
