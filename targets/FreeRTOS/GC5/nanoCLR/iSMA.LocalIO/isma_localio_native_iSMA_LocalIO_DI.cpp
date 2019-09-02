/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */


#include "isma_localio_native.h"
#include "LocalIO_DI.h"


HRESULT Library_isma_localio_native_iSMA_LocalIO_DI::DIGetNative___BOOLEAN( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    {
        CLR_RT_HeapBlock *pThis = stack.This();
        FAULT_ON_NULL(pThis);

        // uint32_t diNumber = pThis[FIELD_dinputId].NumericByRefConst().u4;
        // bool diState = LocalIO_DI_ReadInput((uint8_t)diNumber);
        
        // stack.SetResult_Boolean(diState);
        (void) stack;
        
    }
    NANOCLR_NOCLEANUP();
}
