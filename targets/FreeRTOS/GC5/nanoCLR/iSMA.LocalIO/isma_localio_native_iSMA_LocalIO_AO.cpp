/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_localio_native.h"
#include "LocalIO_AO_TO.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_AO::SetConfigNative___VOID( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___id].NumericByRefConst().u4;
        AO_Mode_t mode = (AO_Mode_t)pThis[FIELD___mode ].NumericByRefConst().u1;
        PWM_Freq_t frequency = (PWM_Freq_t)pThis[FIELD___frequency].NumericByRefConst().u1;

        SetAOMode(aoNo, mode);
        SetAOFrequency(aoNo, frequency);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_AO::SetDutyNative___VOID__U1( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___id].NumericByRefConst().u4;
        uint8_t duty = stack.Arg1().NumericByRef().u1;

        SetAODutyCycle(aoNo, duty);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_AO::SetVoltageNative___VOID__U2( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___id].NumericByRefConst().u4;
        uint16_t voltage = stack.Arg1().NumericByRef().u1;

        SetAOVoltage(aoNo, voltage);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_AO::SetDigitalNative___VOID__BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___id].NumericByRefConst().u4;
        bool value = stack.Arg1().NumericByRef().u1;

        SetAODigital(aoNo, value);
    }
    NANOCLR_NOCLEANUP();
}