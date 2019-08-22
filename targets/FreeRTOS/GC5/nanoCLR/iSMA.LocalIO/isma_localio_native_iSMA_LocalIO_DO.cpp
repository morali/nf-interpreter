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

HRESULT Library_isma_localio_native_iSMA_LocalIO_DO::DOSetNative___VOID__BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t DONo = pThis[FIELD___doutputId].NumericByRefConst().u4;
        bool state = (bool)stack.Arg1().NumericByRef().u1;
        SetDO(state, DONo);

    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_DO::DOGetNative___BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t DONo = pThis[FIELD___doutputId].NumericByRefConst().u4;
        bool state = GetDO(DONo);

        stack.SetResult_Boolean(state);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_localio_native_iSMA_LocalIO_DO::DOToggleNative___BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t DONo = pThis[FIELD___doutputId].NumericByRefConst().u4;
        bool state;
        state = ToggleDO(DONo);
        stack.SetResult_Boolean(state);
    }
    NANOCLR_NOCLEANUP();
}
