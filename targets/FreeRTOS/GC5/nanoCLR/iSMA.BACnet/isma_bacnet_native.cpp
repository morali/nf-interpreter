#include "isma_bacnet_native.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::BacnetObjectAdded___STATIC__VOID__iSMABACnetPartialBacnetObject,
    Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::BacnetObjectRemoved___STATIC__VOID__iSMABACnetPartialBacnetObject,
    Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::UpdateDatabaseRevision___STATIC__VOID,
    NULL,
    NULL,
    Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::SetValueWithPriority___VOID__R4__BOOLEAN__I4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_BACnet =
{
    "iSMA.BACnet", 
    0x1531A88E,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    { 100, 0, 0, 0 }
};
