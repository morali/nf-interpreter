/*
 * Created on Mon Aug 19 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "Panel.h"
#include "isma_gpio_native.h"

HRESULT Library_isma_gpio_native_iSMA_GPIO_DIPSwitch::DIPSwitchGetNative___U1(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t dipNo = pThis[FIELD___switchId].NumericByRefConst().u4;

    uint8_t state = GetDipswitch(dipNo);

    stack.SetResult_U1(state);
  }
  NANOCLR_NOCLEANUP();
}
