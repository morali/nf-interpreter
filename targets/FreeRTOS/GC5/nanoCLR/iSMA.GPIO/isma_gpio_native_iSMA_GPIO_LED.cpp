/*
 * Created on Fri Aug 16 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "Panel.h"
#include "isma_gpio_native.h"

HRESULT Library_isma_gpio_native_iSMA_GPIO_LED::LEDSetNative___VOID__BOOLEAN(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t ledNo = pThis[FIELD___ledId].NumericByRefConst().u4;
    bool state = (bool)stack.Arg1().NumericByRef().u1;

    SetLed(ledNo, state);
  }
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_gpio_native_iSMA_GPIO_LED::LEDGetNative___BOOLEAN(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  {
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    uint32_t ledNo = pThis[FIELD___ledId].NumericByRefConst().u4;
    bool state = GetLed(ledNo);

    stack.SetResult_Boolean(state);
  }
  NANOCLR_NOCLEANUP();
}
