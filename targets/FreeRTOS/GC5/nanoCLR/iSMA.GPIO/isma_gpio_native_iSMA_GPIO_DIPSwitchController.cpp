/*
 * Created on Mon Aug 19 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "Panel.h"
#include "isma_gpio_native.h"

HRESULT Library_isma_gpio_native_iSMA_GPIO_DIPSwitchController::GetDIPSwitchesNative___STATIC__SZARRAY_iSMAGPIODIPSwitch(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index DIPSwitchTypeDef;
  CLR_RT_HeapBlock *dip;
  CLR_RT_HeapBlock *hbObj;
  CLR_RT_HeapBlock &top = stack.PushValue();

  uint32_t dipCount = getDipswitchNumber();

  // find <DIPSwitch> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("DIPSwitch", "iSMA.GPIO", DIPSwitchTypeDef);

  // create an array of <DIPSwitch>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, dipCount, DIPSwitchTypeDef));

  // get a pointer to the first object in the array (which is of type <DIPSwitch>)
  dip = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  if (dipCount > 0) {
    for (uint32_t i = 0; i < dipCount; i++) {
      // create an instance of <DIPSwitch>
      NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*dip, DIPSwitchTypeDef));

      // dereference the object in order to reach its fields
      hbObj = dip->Dereference();

      // get a reference to the switchId managed field...
      CLR_RT_HeapBlock &switchIdFieldRef = hbObj[Library_isma_gpio_native_iSMA_GPIO_DIPSwitch::FIELD___switchId];
      CLR_UINT8 *pRes = &switchIdFieldRef.NumericByRef().u1;
      *pRes = i;

      // DIPSwitch name
      NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_gpio_native_iSMA_GPIO_DIPSwitch::FIELD___name], GetDipswitchName(i)));

      // move the DIPSwitch pointer to the next item in the array
      dip++;
    }
  }

  NANOCLR_NOCLEANUP();
}
