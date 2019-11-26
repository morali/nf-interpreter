#include "isma_deviceinfo_native.h"


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
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetCoreVersion___STRING,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::SetCoreVersion___VOID__STRING,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetStorageType___iSMADeviceInfoDeviceInfoStorageType,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetFirmwareVersion___STRING,
    Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetModelName___STRING,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_DeviceInfo =
{
    "iSMA.DeviceInfo", 
    0x1C096975,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    { 100, 0, 0, 0 }
};
