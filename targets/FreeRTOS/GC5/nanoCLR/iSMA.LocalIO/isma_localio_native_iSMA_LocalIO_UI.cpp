/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "MCP3421.h"
#include "isma_localio_native.h"
#include "LocalIO_UI.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_UI::SetConfigNative___VOID(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t uiNo = pThis[FIELD___id].NumericByRefConst().u4;
    Samplerate_t resolution = (Samplerate_t)pThis[FIELD___resolution].NumericByRefConst().u1;
    PGAGain_t gain = (PGAGain_t)pThis[FIELD___gain].NumericByRefConst().u1;
    uint8_t filterTime = pThis[FIELD___filterTime].NumericByRefConst().u1;
    bool measureVoltage = pThis[FIELD___measureVoltage].NumericByRefConst().u1;
    bool measureResistance = pThis[FIELD___measureResistance].NumericByRefConst().u1;

    SetUIResolution(uiNo, resolution);
    SetUIGain(uiNo, gain);
    SetUIFilter(uiNo, filterTime);
    SetUIMeasureVoltage(uiNo, measureVoltage);
    SetUIMeasureResistance(uiNo, measureResistance);
  }
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_UI::GetVoltageNative___I2(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t uiNo = pThis[FIELD___id].NumericByRefConst().u4;
    int16_t voltage = GetUIVoltage(uiNo);

    stack.SetResult_I2(voltage);
  }
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_UI::GetResistanceNative___U4(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t uiNo = pThis[FIELD___id].NumericByRefConst().u4;
    uint32_t res = GetUIResistance(uiNo);

    stack.SetResult_U4(res);
  }
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_UI::GetDigitalNative___BOOLEAN(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t uiNo = pThis[FIELD___id].NumericByRefConst().u4;
    bool state = GetUIDigital(uiNo);

    stack.SetResult_Boolean(state);
  }
  NANOCLR_NOCLEANUP();
}