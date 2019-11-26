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

// void set_model_version(char *model);
// void set_firmware_version(char *firmware);
struct Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo {
  static const int FIELD_STATIC__implementation = 0;

  //--//
};

struct Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi {
  NANOCLR_NATIVE_DECLARE(GetCoreVersion___STRING);
  NANOCLR_NATIVE_DECLARE(SetCoreVersion___VOID__STRING);
  NANOCLR_NATIVE_DECLARE(GetStorageType___iSMADeviceInfoDeviceInfoStorageType);
  NANOCLR_NATIVE_DECLARE(GetFirmwareVersion___STRING);
  NANOCLR_NATIVE_DECLARE(GetModelName___STRING);

  //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_DeviceInfo;

#endif //_ISMA_DEVICEINFO_NATIVE_H_