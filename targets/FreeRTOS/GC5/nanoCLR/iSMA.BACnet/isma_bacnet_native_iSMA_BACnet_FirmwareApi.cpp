

#include "device.h"
#include "isma_bacnet_native.h"
#include "isma_bacnet_objects.h"
#include "isma_bacnet_objects_helper.h"

bacObj_Device_t *device_object;

bacObj_AV_t *analog_listHead;

bacObj_Device_t *getDeviceObject() { return device_object; }

bool addDevice_Object(CLR_RT_HeapBlock *objBlock) {
  uint8_t success = false;

  device_object = (bacObj_Device_t *)malloc(sizeof(bacObj_Device_t));

  if (device_object == NULL)
    return false;

  device_object->objBlock = (void *)objBlock;
  device_object->gc = (void *)new CLR_RT_ProtectFromGC(*objBlock);

  if (device_object->gc == NULL) {
    free(device_object);
    return false;
  }

  uint32_t set_value = 826;

  /* Set firmware defined variables */
  setDeviceValue(_vendorName, (void*)"GC5");
  setDeviceValue(_vendorId, (void*)&set_value);
  setDeviceValue(_modelName, (void *)"RC18");
  setDeviceValue(_firmwareRevision, (void*)"WIP");
  set_value = 14;
  setDeviceValue(_protocolVersion, (void*)&set_value);
  set_value = 1;
  setDeviceValue(_protocolRevision, (void*)&set_value);

  return success;
}

bool addAnalogValue_Object(CLR_RT_HeapBlock *bacObj) {
  uint8_t success = false;

  bacObj_AV_t *newObj = (bacObj_AV_t *)malloc(sizeof(bacObj_AV_t));

  if (newObj == NULL)
    return false;

  newObj->objBlock = (void *)bacObj;
  newObj->gc = (void *)new CLR_RT_ProtectFromGC(*bacObj);
  newObj->next = NULL;

  if (newObj->gc == NULL) {
    free(newObj);
    return false;
  }

  if (analog_listHead == NULL) {
    analog_listHead = newObj;
  }

  analog_listHead->next = newObj;
  analog_listHead = newObj;

  return success;
}

HRESULT Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::BacnetObjectAdded___STATIC__VOID__iSMABACnetPartialBacnetObject(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  // get bacnet object

  CLR_RT_HeapBlock *objBlock = stack.Arg0().Dereference();

  uint32_t type = objBlock[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType].NumericByRefConst().u4;

  (void)type;
  switch (type) {
  case 8:
    addDevice_Object(objBlock);
    break;
  default:
    addAnalogValue_Object(objBlock);
    break;
  }

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::BacnetObjectRemoved___STATIC__VOID__iSMABACnetPartialBacnetObject(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::UpdateDatabaseRevision___STATIC__VOID(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  Device_Inc_Database_Revision();
  (void)stack;
  NANOCLR_NOCLEANUP_NOLABEL();
}
