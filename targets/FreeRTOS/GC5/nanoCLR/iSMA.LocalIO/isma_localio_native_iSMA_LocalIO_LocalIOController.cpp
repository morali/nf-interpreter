/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "LocalIO_AO_TO.h"
#include "LocalIO_DI.h"
#include "LocalIO_DO.h"
#include "LocalIO_UI.h"
#include "isma_localio_native.h"

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDIsNative___STATIC__SZARRAY_iSMALocalIODI(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  CLR_RT_TypeDef_Index DITypeDef;
  CLR_RT_HeapBlock *DI;
  CLR_RT_HeapBlock *hbObj;
  CLR_RT_HeapBlock &top = stack.PushValue();

  uint32_t diCount = GetDINumber();

  // find <DI> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("DI", "iSMA.LocalIO", DITypeDef);

  // create an array of <DI>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, diCount, DITypeDef));

  // get a pointer to the first object in the array (which is of type <DI>)
  DI = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  if (diCount > 0) {
    for (uint32_t i = 0; i < diCount; i++) {
      // create an instance of <DI>
      NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*DI, DITypeDef));

      // dereference the object in order to reach its fields
      hbObj = DI->Dereference();

      // get a reference to the DIId managed field...
      CLR_RT_HeapBlock &DIIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_DI::FIELD___id];
      CLR_UINT8 *pRes = &DIIdFieldRef.NumericByRef().u1;
      *pRes = i;

      char name[] = "DIx";
      name[2] = i + 1 + '0';

      // DI name
      NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_localio_native_iSMA_LocalIO_DI::FIELD___name], name));

      // move the DI pointer to the next item in the array
      DI++;
    }
  }
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

	if (douputCount > 0) 
	{
		for (uint32_t i = 0; i < douputCount; i++) 
		{
			// create an instance of <DO>
			NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*DO, DOTypeDef));

			// dereference the object in order to reach its fields
			hbObj = DO->Dereference();

			// get a reference to the DOId managed field...
			CLR_RT_HeapBlock &DOIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_DO::FIELD___id];
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

	uint32_t uiCount = GetUINumber();

	// find <UI> type, don't bother checking the result as it exists for sure
	g_CLR_RT_TypeSystem.FindTypeDef("UI", "iSMA.LocalIO", UITypeDef);

	// create an array of <UI>
	NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, uiCount, UITypeDef));

	// get a pointer to the first object in the array (which is of type <UI>)
	UI = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

	if (uiCount > 0) 
	{
		for (uint32_t i = 0; i < uiCount; i++)
		{
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

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetAOsNative___STATIC__SZARRAY_iSMALocalIOAO(CLR_RT_StackFrame &stack) {
	NANOCLR_HEADER();

	CLR_RT_TypeDef_Index AOTypeDef;
	CLR_RT_HeapBlock *AO;
	CLR_RT_HeapBlock *hbObj;
	CLR_RT_HeapBlock &top = stack.PushValue();

	uint32_t aoCount = GetAONumber();

	// find <AO> type, don't bother checking the result as it exists for sure
	g_CLR_RT_TypeSystem.FindTypeDef("AO", "iSMA.LocalIO", AOTypeDef);

	// create an array of <AO>
	NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, aoCount, AOTypeDef));

	// get a pointer to the first object in the array (which is of type <AO>)
	AO = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

	if (aoCount > 0) 
	{
		for (uint32_t i = 0; i < aoCount; i++)
		{
			// create an instance of <AO>
			NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*AO, AOTypeDef));

			// dereference the object in order to reach its fields
			hbObj = AO->Dereference();

			// get a reference to the AOId managed field...
			CLR_RT_HeapBlock &AOIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_AO::FIELD___id];
			CLR_UINT8 *pRes = &AOIdFieldRef.NumericByRef().u1;
			*pRes = i;

			char name[] = "AOx";
			name[2] = i + 1 + '0';

			// AO name
			NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_localio_native_iSMA_LocalIO_AO::FIELD___name], name));

			// move the AO pointer to the next item in the array
			AO++;
		}
	}
	NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetTOsNative___STATIC__SZARRAY_iSMALocalIOTO(CLR_RT_StackFrame &stack) {
	NANOCLR_HEADER();

	CLR_RT_TypeDef_Index TOTypeDef;
	CLR_RT_HeapBlock *TO;
	CLR_RT_HeapBlock *hbObj;
	CLR_RT_HeapBlock &top = stack.PushValue();

	uint32_t toCount = GetTONumber();

	// find <TO> type, don't bother checking the result as it exists for sure
	g_CLR_RT_TypeSystem.FindTypeDef("TO", "iSMA.LocalIO", TOTypeDef);

	// create an array of <TO>
	NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, toCount, TOTypeDef));

	// get a pointer to the first object in the array (which is of type <TO>)
	TO = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

	if (toCount > 0) 
	{
		for (uint32_t i = 0; i < toCount; i++)
		{
			// create an instance of <TO>
			NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*TO, TOTypeDef));

			// dereference the object in order to reach its fields
			hbObj = TO->Dereference();

			// get a reference to the TOId managed field...
			CLR_RT_HeapBlock &TOIdFieldRef = hbObj[Library_isma_localio_native_iSMA_LocalIO_TO::FIELD___id];
			CLR_UINT8 *pRes = &TOIdFieldRef.NumericByRef().u1;
			*pRes = i;

			char name[] = "TOx";
			name[2] = i + 1 + '0';

			// TO name
			NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_localio_native_iSMA_LocalIO_TO::FIELD___name], name));

			// move the TO pointer to the next item in the array
			TO++;
		}
	}
	NANOCLR_NOCLEANUP();
}