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

#include "isma_deviceinfo_native.h"
#include <corlib_native.h>

char core_version[40] = "Core version not set!";
char model_version[40] = "Model version not set!";
char firmware_version[40] = "Firmware version not set!";

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetCoreVersion___STRING(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  /* because the caller is expecting a result to be returned */
  /* we need set a return result in the stack argument using the a apropriate SetResult according to the variable type (a string here) */
  stack.SetResult_String(core_version);

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::SetCoreVersion___VOID__STRING(CLR_RT_StackFrame &stack) {
  
  NANOCLR_HEADER();

  /* dereference string buffer from the argument */
  const char *data = stack.Arg1().DereferenceString()->StringText();

  if (data != NULL) {

    uint8_t length = hal_strlen_s(data);
    hal_strncpy_s(core_version, sizeof(core_version), data, length);
  }

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetStorageType___iSMADeviceInfoDeviceInfoStorageType(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetFirmwareVersion___STRING(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  (void) stack;

  stack.SetResult_String(firmware_version);

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_FirmwareApi::GetModelName___STRING(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  (void) stack;

  stack.SetResult_String(model_version);

  NANOCLR_NOCLEANUP_NOLABEL();
}

void set_model_version(char *model) {
  if (model != NULL) {

    uint8_t length = hal_strlen_s(model);
    hal_strncpy_s(model_version, sizeof(model_version), model, length);
  }
}

void set_firmware_version(char *firmware) {
  if (firmware != NULL) {

    uint8_t length = hal_strlen_s(firmware);
    hal_strncpy_s(firmware_version, sizeof(firmware_version), firmware, length);
  }
}