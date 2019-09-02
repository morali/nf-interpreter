/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_localio_native.h"
#include "LocalIO_AO.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_AO::SetConfigNative___VOID( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t aoNo = pThis[FIELD___aoutputId].NumericByRefConst().u4;
        uint16_t voltage = pThis[FIELD___voltage].NumericByRefConst().u1;
        bool pwm = pThis[FIELD___pwm ].NumericByRefConst().u1;
        uint32_t frequency = pThis[FIELD___frequency].NumericByRefConst().u1;
        uint32_t duty = pThis[FIELD___duty].NumericByRefConst().u1;

        SetAOVoltage(aoNo, voltage);
        SetAOPWM(aoNo, pwm);
        SetAOFrequency(aoNo, frequency);
        SetAODutyCycle(aoNo, duty);
    }
    NANOCLR_NOCLEANUP();
}

