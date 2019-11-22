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

char core_version[40] = "Core version not set!";

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo::GetCoreVersionNative___STATIC__STRING(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();
  /* because the caller is expecting a result to be returned */
  /* we need set a return result in the stack argument using the a ppropriate SetResult according to the variable type (a string here) */
  stack.SetResult_String(core_version);

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo::SetCoreVersionNative___STATIC__VOID__STRING(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  /* get a pointer to the managed object instance and check that it's not NULL */
  //   CLR_RT_HeapBlock *pThis = stack.This();
  //   FAULT_ON_NULL(pThis);

  /* dereference string buffer from the argument */
  CLR_RT_HeapBlock_Array *stringBuffer = stack.Arg0().DereferenceArray();
  FAULT_ON_NULL(stringBuffer);

  const char *data = stringBuffer->StringText();

  FAULT_ON_NULL(data);

  uint8_t length = stringBuffer->m_numOfElements;

  hal_strncpy_s(core_version, 40, data, length);

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo::GetStorageType___STATIC__iSMADeviceInfoDeviceInfoStorageType(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

  NANOCLR_NOCLEANUP();
}
