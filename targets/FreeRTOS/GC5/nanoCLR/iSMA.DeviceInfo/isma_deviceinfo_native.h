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
#ifndef _ISMA_DEVICEINFO_NATIVE_H_
#define _ISMA_DEVICEINFO_NATIVE_H_

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>

void set_model_version(char *model);
void set_firmware_version(char *firmware);

struct Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo 
{
  NANOCLR_NATIVE_DECLARE(GetCoreVersionNative___STATIC__STRING);
  NANOCLR_NATIVE_DECLARE(GetFirmwareVersionNative___STATIC__STRING);
  NANOCLR_NATIVE_DECLARE(GetModelVersionNative___STATIC__STRING);
  NANOCLR_NATIVE_DECLARE(SetCoreVersionNative___STATIC__VOID__STRING);
  NANOCLR_NATIVE_DECLARE(GetStorageType___STATIC__iSMADeviceInfoDeviceInfoStorageType);

  //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_DeviceInfo;

#endif //_ISMA_DEVICEINFO_NATIVE_H_