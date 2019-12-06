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

#include "isma_bacnet_native.h"
#include "isma_bacnet_objects_helper.h"

HRESULT Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::SetValueWithPriority___VOID__R4__BOOLEAN__I4( CLR_RT_StackFrame& stack )
{
    NANOCLR_HEADER();
    float float_value = stack.Arg1().NumericByRef().r4;
    uint8_t bool_value = stack.Arg2().NumericByRef().u1;
    uint8_t priority = stack.Arg3().NumericByRef().u1;

    CLR_RT_HeapBlock *pThis = stack.This();

    uint32_t id = pThis[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectIdentifier].NumericByRefConst().u4;

    Set_AnalogValue(id, float_value, bool_value, priority - 1);

    NANOCLR_NOCLEANUP_NOLABEL();
}
