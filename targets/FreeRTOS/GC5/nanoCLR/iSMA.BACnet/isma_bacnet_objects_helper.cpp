
#include "isma_bacnet_objects_helper.h"
#include "isma_bacnet_native.h"
#include "isma_bacnet_objects.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  It needs to be run every time something changes variables
 *         which are avaliable by high level application.
 * @note
 * @retval None
 */
void UpdatePending(CLR_RT_HeapBlock *heap_block) {
  CLR_RT_HeapBlock &update_pending = heap_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___isUpdatePending];
  uint32_t value = 1;
  update_pending.NumericByRef().u4 = value;
}

uint32_t getDeviceValue(object_deviceValues_t var, void *address) {
  bacObj_Device_t *object_device = getDeviceObject();
  CLR_RT_HeapBlock *device_block = (CLR_RT_HeapBlock *)object_device->objBlock;

  uint32_t value = 0;
  switch (var) {
  case _updatePending: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___isUpdatePending].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
    return 1;
    break;
  }
  case _identifier: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectIdentifier].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _name: {
    *(char **)address = (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectName].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _type: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _systemStatus: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___systemStatus].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _vendorName: {
    *(char **)address = (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___vendorName].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _modelName: {
    *(char **)address = (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___modelName].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _firmwareRevision: {
    *(char **)address =
        (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___firmwareRevision].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _applicationSoftwareRevision: {
    *(char **)address =
        (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___applicationSoftwareRevision].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _location: {
    *(char **)address = (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___location].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _device_description: {
    *(char **)address = (char *)device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___description].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _protocolVersion: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___protocolVersion].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _protocolRevision: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___protocolRevision].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _vendorId: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___vendorId].NumericByRefConst().u2;
    memcpy(address, (void *)&value, 2);
    return 1;
    break;
  }
  case _apduRetries: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___apduRetries].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _apduTimeout: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___apduTimeout].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  default: {
    return 0;
    break;
  }
  }
  return 0;
}

void setDeviceValue(object_deviceValues_t var, void *value) {
  bacObj_Device_t *object_device = getDeviceObject();
  CLR_RT_HeapBlock *device_block = (CLR_RT_HeapBlock *)object_device->objBlock;

  UpdatePending(device_block);
  switch (var) {
  case _updatePending: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___isUpdatePending];
    id.NumericByRef().u1 = *(uint8_t *)value;
    break;
  }
  case _name: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectName], (const char *)value);
    break;
  }
  case _type: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType];
    id.NumericByRef().u4 = *(uint32_t *)value;

    break;
  }
  case _systemStatus: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___systemStatus];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _vendorName: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___vendorName], (const char *)value);
    break;
  }
  case _modelName: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___modelName], (const char *)value);
    break;
  }
  case _firmwareRevision: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___firmwareRevision],
                                            (const char *)value);
    break;
  }
  case _applicationSoftwareRevision: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___applicationSoftwareRevision],
                                            (const char *)value);
    break;
  }
  case _location: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___location], (const char *)value);
    break;
  }
  case _device_description: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___description], (const char *)value);
    break;
  }
  case _protocolVersion: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___protocolVersion];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _protocolRevision: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___protocolRevision];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _vendorId: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___vendorId];
    id.NumericByRef().u2 = *(uint16_t *)value;
    break;
  }
  case _apduRetries: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___apduRetries];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _apduTimeout: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___apduTimeout];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  default:
    break;
  }
}

float *Extract_Float(uint32_t object_instance) {

  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  if (av_instance == NULL)
    return NULL;
  CLR_RT_HeapBlock_Array *av_block =
      ((CLR_RT_HeapBlock *)av_instance->objBlock)[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___values].DereferenceArray();
  if (av_block == NULL)
    return NULL;
  float *value = (float *)av_block->GetFirstElement();
  if (value == NULL)
    return NULL;
  return value;
}

bool *Extract_Bool(uint32_t object_instance) {

  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  if (av_instance == NULL)
    return NULL;
  CLR_RT_HeapBlock_Array *av_block =
      ((CLR_RT_HeapBlock *)av_instance->objBlock)[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___valuesReality].DereferenceArray();
  if (av_block == NULL)
    return NULL;
  bool *value = (bool *)av_block->GetFirstElement();
  if (value == NULL)
    return NULL;
  return value;
}

float Get_PresentValue(uint32_t object_instance) {

  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  float return_value = 0;
  float *value = Extract_Float(object_instance);
  bool *value_not_null = Extract_Bool(object_instance);

  if (value == NULL || value_not_null == NULL)
    return return_value;

  return_value = value[16]; /* relinquish defaults (17th priority) */
  for (uint16_t i = 0; i < BACNET_MAX_PRIORITY; i++) {
    if (value_not_null[i] == true) {
      return_value = value[i];
      setAnalogValue(_presentValue, (void *)&return_value, av_instance);
      setAnalogValue(_presentPriority, (void *)&i, av_instance);
      break;
    }
  }
  return return_value;
}

/**
 * For a given object instance-number, sets the present-value at a given
 * priority 1..16.
 *
 * @param  object_instance - object-instance number of the object
 * @param  value - floating point analog value
 * @param  priority - priority 1..16
 *
 * @return  true if values are within range and present-value is set.
 */
bool Set_AnalogValue(uint32_t object_instance, float incoming_float, bool incoming_bool, uint8_t priority) {

  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  float *value = Extract_Float(object_instance);
  bool *value_not_null = Extract_Bool(object_instance);

  if (value == NULL || value_not_null == NULL)
    return NULL;

  value[priority - 1] = incoming_float;
  value_not_null[priority - 1] = incoming_bool;

  /* We call this function to update present value */
  Get_PresentValue(object_instance);
  uint8_t set_bit = 1;
  setAnalogValue(_av_updatePending, (void *)&set_bit, av_instance);
  return true;
}

uint32_t getAnalogValue(object_analogValues_t var, void *address, bacObj_AV_t *object_analog) {
  CLR_RT_HeapBlock *analog_block = (CLR_RT_HeapBlock *)object_analog->objBlock;

  uint32_t value = 0;
  float float_value = 0xFFFFFFFF;
  switch (var) {
  case _av_updatePending: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___isUpdatePending].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
    return 1;
    break;
  }
  case _av_identifier: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectIdentifier].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _av_name: {
    *(char **)address = (char *)analog_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectName].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _av_type: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _presentValue: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___presentValue].NumericByRefConst().r4;
    memcpy(address, (void *)&float_value, 4);
    return 1;
  }
  case _av_description: {
    *(char **)address =
        (char *)analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___description].DereferenceString()->StringText();
    return hal_strlen_s((const char *)address);
    break;
  }
  case _statusFlags: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___statusFlags].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
    return 1;
    break;
  }
  case _reliability: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___reliability].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _units: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___units].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
    return 1;
    break;
  }
  case _covIncrement: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___covIncrement].NumericByRefConst().r4;
    memcpy(address, (void *)&float_value, 4);
    return 1;
    break;
  }
  case _presentPriority: {
    value = analog_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___presentPriority].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
    return 1;
    break;
  }
  default: {
    return 0;
    break;
  }
  }
  return 0;
}

void setAnalogValue(object_analogValues_t var, void *value, bacObj_AV_t *object_analog) {
  CLR_RT_HeapBlock *device_block = (CLR_RT_HeapBlock *)object_analog->objBlock;

  UpdatePending(device_block);
  switch (var) {
  case _av_identifier: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectIdentifier];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _av_name: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectName], (const char *)value);
    break;
  }
  case _av_type: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectType];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _presentValue: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___presentValue];
    id.NumericByRef().r4 = *(uint8_t *)value;
    break;
  }
  case _av_description: {
    CLR_RT_HeapBlock_String::CreateInstance(device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___description],
                                            (const char *)value);
    break;
  }
  case _statusFlags: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___statusFlags];
    id.NumericByRef().u1 = *(uint32_t *)value;
    break;
  }
  case _reliability: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___reliability];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _units: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___units];
    id.NumericByRef().u4 = *(uint32_t *)value;
    break;
  }
  case _covIncrement: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___covIncrement];
    id.NumericByRef().r4 = *(uint32_t *)value;
    break;
  }
  case _presentPriority: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO::FIELD___presentPriority];
    id.NumericByRef().u1 = *(uint32_t *)value;
    break;
  }
  default: {
    break;
  }
  }
}

#ifdef __cplusplus
}
#endif