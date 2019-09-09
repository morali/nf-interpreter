/*
 * Created on Wed Sep 04 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "LocalIO_AO_TO.h"
#include "isma_localio_native.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_TO::SetConfigNative___VOID( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t id = pThis[FIELD___id].NumericByRefConst().u4;
        PWM_Freq_t frequency = (PWM_Freq_t)pThis[FIELD___frequency].NumericByRefConst().u1;
        TO_Mode_t mode = (TO_Mode_t)pThis[FIELD___mode].NumericByRefConst().u1;

        SetTOFrequency(id, frequency);
        SetTOMode(id, mode);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_TO::SetDutyNative___VOID__U1( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t id = pThis[FIELD___id].NumericByRefConst().u4;
        uint8_t duty = stack.Arg1().NumericByRef().u1;

        SetTODutyCycle(id, duty);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_TO::SetDigitalNative___VOID__BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
{
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t id = pThis[FIELD___id].NumericByRefConst().u4;
        bool value = stack.Arg1().NumericByRef().u1;

        SetTODigital(id, value);
    }
    NANOCLR_NOCLEANUP();
}