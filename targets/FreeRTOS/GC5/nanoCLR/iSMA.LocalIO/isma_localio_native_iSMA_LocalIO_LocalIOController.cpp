/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_localio_native.h"
#include "LocalIO_DO.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDIsNative___STATIC__SZARRAY_iSMALocalIODI(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

   CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);
  // CLR_RT_TypeDef_Index DITypeDef;
  // CLR_RT_HeapBlock *DI;
  // CLR_RT_HeapBlock *hbObj;
  // CLR_RT_HeapBlock &top = stack.PushValue();

  // uint32_t diCount = DIGITAL_INPUTS_AMOUNT;

  // // find <DO> type, don't bother checking the result as it exists for sure
  // g_CLR_RT_TypeSystem.FindTypeDef("DI", "iSMA.LocalIO", DITypeDef);

  // // create an array of <DO>
  // NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, diCount, DITypeDef));

  // // get a pointer to the first object in the array (which is of type <DO>)
  // DI = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  // if (diCount > 0) {
  //   for (uint32_t i = 0; i < diCount; i++) {
  //     // create an instance of <DO>
  //     NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*DI, DITypeDef));

  //     // dereference the object in order to reach its fields
  //     hbObj = DI->Dereference();

  //     // get a reference to the DOId managed field...
  //     CLR_RT_HeapBlock &DIIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_DI::FIELD___doutputId];
  //     CLR_UINT8 *pRes = &DIIdFieldRef.NumericByRef().u1;
  //     *pRes = i;

  //     char name[] = "DIx";
  //     name[2] = i + 1 + '0';

  //     // DO name
  //     NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_localio_native_iSMA_LocalIO_DI::FIELD___name], name));

  //     // move the DO pointer to the next item in the array
  //     DI++;
  (void) stack;
  //   }
  // }
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDOsNative___STATIC__SZARRAY_iSMALocalIODO(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  CLR_RT_TypeDef_Index DOTypeDef;
  CLR_RT_HeapBlock *DO;
  CLR_RT_HeapBlock *hbObj;
  CLR_RT_HeapBlock &top = stack.PushValue();

  uint32_t douputCount = GetDONumber();

  // find <DO> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("DO", "iSMA.LocalIO", DOTypeDef);

  // create an array of <DO>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, douputCount, DOTypeDef));

  // get a pointer to the first object in the array (which is of type <DO>)
  DO = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  if (douputCount > 0) {
    for (uint32_t i = 0; i < douputCount; i++) {
      // create an instance of <DO>
      NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*DO, DOTypeDef));

      // dereference the object in order to reach its fields
      hbObj = DO->Dereference();

      // get a reference to the DOId managed field...
      CLR_RT_HeapBlock &DOIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_DO::FIELD___doutputId];
      CLR_UINT8 *pRes = &DOIdFieldRef.NumericByRef().u1;
      *pRes = i;

      char name[] = "DOx";
      name[2] = i + 1 + '0';

      // DO name
      NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_localio_native_iSMA_LocalIO_DO::FIELD___name], name));

      // move the DO pointer to the next item in the array
      DO++;
    }
  }
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetUIsNative___STATIC__SZARRAY_iSMALocalIOUI(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index UITypeDef;
  CLR_RT_HeapBlock *UI;
  CLR_RT_HeapBlock *hbObj;
  CLR_RT_HeapBlock &top = stack.PushValue();

  uint32_t UINumber = 0;
  uint32_t uiCount = UINumber;

  // find <UI> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("UI", "iSMA.LocalIO", UITypeDef);

  // create an array of <UI>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, uiCount, UITypeDef));

  // get a pointer to the first object in the array (which is of type <UI>)
  UI = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  if (uiCount > 0) {
    for (uint32_t i = 0; i < uiCount; i++) {
      // create an instance of <UI>
      NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*UI, UITypeDef));

      // dereference the object in order to reach its fields
      hbObj = UI->Dereference();

      // get a reference to the UIId managed field...
      CLR_RT_HeapBlock &UIIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_UI::FIELD___id];
      CLR_UINT8 *pRes = &UIIdFieldRef.NumericByRef().u1;
      *pRes = i;

      char name[] = "UIx";
      name[2] = i + 1 + '0';

      // UI name
      NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_localio_native_iSMA_LocalIO_UI::FIELD___name], name));

      // move the UI pointer to the next item in the array
      UI++;
    }
  }

  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetAONative___STATIC__SZARRAY_iSMALocalIOAO(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

  NANOCLR_NOCLEANUP();
}
