//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#include "isma_localio_native.h"


HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDIsNative___STATIC__SZARRAY_iSMALocalIODI( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();

    NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDOsNative___STATIC__SZARRAY_iSMALocalIODO( CLR_RT_StackFrame& stack )
{
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

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetUIsNative___STATIC__SZARRAY_iSMALocalIOUI( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();

    NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetAONative___STATIC__SZARRAY_iSMALocalIOAO( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();

    NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

    NANOCLR_NOCLEANUP();
}
