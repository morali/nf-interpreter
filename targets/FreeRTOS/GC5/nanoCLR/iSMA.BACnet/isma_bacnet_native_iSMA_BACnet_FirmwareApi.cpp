

#include "GlobalEventsFlags.h"
#include "device.h"
#include "isma_bacnet_native.h"
#include "isma_bacnet_objects.h"
#include "isma_bacnet_objects_helper.h"

bacObj_Device_t *device_object;

bacObj_AV_t *analog_listHead;
bacObj_AV_t *analog_listTail;

bacObj_Device_t *Get_DeviceObject() { return device_object; }
bacObj_AV_t *Get_AnalogValue_Head() { return analog_listHead; }

bacObj_AV_t *Get_AnalogValue_ByIndex(uint32_t index) {
  bacObj_AV_t *head = Get_AnalogValue_Head();
  uint32_t id = 0;
  while (head != NULL) {
    Get_AnalogValue(_av_identifier, (void *)&id, head);
    if (id == index)
      return head;
    head = head->next;
  }
  return NULL;
}

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

  xEventGroupSetBits(xGlobalEventsFlags, EVENT_DEVICE_OK);

  return success;
}

bool addAnalogValue_Object(CLR_RT_HeapBlock *bacObj) {
  uint8_t success = false;

  bacObj_AV_t *newObj = (bacObj_AV_t *)malloc(sizeof(bacObj_AV_t));

  if (newObj == NULL)
    return success;

  memset(newObj, 0, sizeof(bacObj_AV_t));

  newObj->objBlock = (void *)bacObj;
  newObj->gc = new CLR_RT_ProtectFromGC(*bacObj);
  newObj->next = NULL;

  if (newObj->gc == NULL) {
    free(newObj);
    return success;
  }

  /* Find first free index */
  uint32_t i = 0;
  while (true) {
    if (Get_AnalogValue_ByIndex(i) == NULL) {
      Set_AnalogValue(_av_identifier, (void *)&i, newObj);
      break;
    };
    i++;
  }

  if (analog_listHead == NULL) {
    analog_listHead = newObj;
    analog_listTail = newObj;
    return success;
  }

  analog_listTail->next = newObj;
  newObj->prev = analog_listTail;
  analog_listTail = newObj;

  success = true;
  return success;
}

bool removeAnalogValue_Object(CLR_RT_HeapBlock *bacObj) {
  uint8_t success = false;

  uint32_t value = 0;
  value = bacObj[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectIdentifier].NumericByRefConst().u4;

  bacObj_AV_t *av_obj = Get_AnalogValue_ByIndex(value);
  if (av_obj == NULL)
    return success;

  /* first object on list */
  if (av_obj->prev == NULL) {
    av_obj->next->prev = NULL;
    analog_listHead = av_obj->next;
  }

  /* last object on list */
  else if (av_obj->next == NULL) {
    av_obj->prev->next = NULL;
    analog_listTail = av_obj->prev;
  }

  else {
    av_obj->prev->next = av_obj->next;
    av_obj->next->prev = av_obj->prev;
  }
  delete (CLR_RT_ProtectFromGC *)av_obj->gc;
  free(av_obj);

  success = true;

  return success;
}

HRESULT Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::BacnetObjectAdded___STATIC__VOID__iSMABACnetPartialBacnetObject(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  // get bacnet object

  CLR_RT_HeapBlock *objBlock = stack.Arg0().Dereference();

  uint32_t type = objBlock[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType].NumericByRefConst().u4;

  switch (type) {
  case 8:
    addDevice_Object(objBlock);
    break;
  case 2:
    addAnalogValue_Object(objBlock);
  default:
    break;
  }

  Device_Inc_Database_Revision();

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::BacnetObjectRemoved___STATIC__VOID__iSMABACnetPartialBacnetObject(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  // get bacnet object to remove

  CLR_RT_HeapBlock *objBlock = stack.Arg0().Dereference();

  uint32_t type = objBlock[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType].NumericByRefConst().u4;

  switch (type) {
  case 8:
    break;
  case 2:
    removeAnalogValue_Object(objBlock);
  default:
    break;
  }

  Device_Inc_Database_Revision();

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi::UpdateDatabaseRevision___STATIC__VOID(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  Device_Inc_Database_Revision();
  (void)stack;
  NANOCLR_NOCLEANUP_NOLABEL();
}
