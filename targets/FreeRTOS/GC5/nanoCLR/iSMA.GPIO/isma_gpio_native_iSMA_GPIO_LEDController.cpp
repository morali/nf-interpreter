/*
 * Created on Fri Aug 16 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_gpio_native.h"

#include "Panel.h"

HRESULT Library_isma_gpio_native_iSMA_GPIO_LEDController::GetLedsNative___STATIC__SZARRAY_iSMAGPIOLED(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index LEDTypeDef;
  CLR_RT_HeapBlock *led;
  CLR_RT_HeapBlock *hbObj;
  CLR_RT_HeapBlock &top = stack.PushValue();

  uint32_t ledCount = getLedNumber();

  // find <LED> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("LED", "iSMA.GPIO", LEDTypeDef);

  // create an array of <LED>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, ledCount, LEDTypeDef));

  // get a pointer to the first object in the array (which is of type <LED>)
  led = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  if (ledCount > 0) {
    for (uint32_t i = 0; i < ledCount; i++) {
      // create an instance of <LED>
      NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*led, LEDTypeDef));

      // dereference the object in order to reach its fields
      hbObj = led->Dereference();

      // get a reference to the ledId managed field...
      CLR_RT_HeapBlock &ledIdFieldRef = hbObj[Library_isma_gpio_native_iSMA_GPIO_LED::FIELD___ledId];
      CLR_UINT8 *pRes = &ledIdFieldRef.NumericByRef().u1;
      *pRes = i;

      // LED name
      NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_gpio_native_iSMA_GPIO_LED::FIELD___name], GetLedName(i)));

      // move the LED pointer to the next item in the array
      led++;
    }
  }

  NANOCLR_NOCLEANUP();
}
