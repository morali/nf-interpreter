#include "isma_deviceinfo_native.h"

static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo::GetCoreVersionNative___STATIC__STRING,
    NULL,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo::SetCoreVersionNative___STATIC__VOID__STRING,
    NULL,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo::GetStorageType___STATIC__iSMADeviceInfoDeviceInfoStorageType,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_DeviceInfo =
{
    "iSMA.DeviceInfo", 
    0x115BFD45,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    {100, 0, 0, 0}
};
