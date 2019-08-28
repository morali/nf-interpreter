/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */


#include "isma_localio_native.h"
#include "LocalIO_DO.h"

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

HRESULT Library_isma_localio_native_iSMA_LocalIO_DO::DOToggleNative___VOID( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        uint32_t DONo = pThis[FIELD___doutputId].NumericByRefConst().u4;
        ToggleDO(DONo);
    }
    NANOCLR_NOCLEANUP();
}
