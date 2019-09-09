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
        uint16_t voltage = pThis[FIELD___voltage].NumericByRefConst().u1;
        bool mode = pThis[FIELD___mode ].NumericByRefConst().u1;
        PWM_Freq_t frequency = (PWM_Freq_t)pThis[FIELD___frequency].NumericByRefConst().u1;

        SetAOVoltage(aoNo, voltage);
        SetAOPWM(aoNo, mode);
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