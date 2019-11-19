
#include "isma_bacnet_objects_helper.h"
#include "isma_bacnet_native.h"
#include "isma_bacnet_objects.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Disable optimalizations for testing purposes */
#pragma GCC push_options
#pragma GCC optimize("O0")

/**
 * @brief  It needs to be run every time something changes variables
 *         which are avaliable by high level application.
 * @note
 * @retval None
 */
void setUpdatePending(void) {
  bacObj_Device_t *object_device = getDeviceObject();
  CLR_RT_HeapBlock *device_block = (CLR_RT_HeapBlock *)object_device->objBlock;

  uint32_t value = 1;
  CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___isUpdatePending];
  id.NumericByRef().u1 = *(uint32_t *)value;
}

uint32_t getBaseValue(object_baseValues_t var, void *address) {
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
  default: {
    return 0;
    break;
  }
  }
  return 0;
}

void setBaseValue(object_baseValues_t var, void *value) {
  bacObj_Device_t *object_device = getDeviceObject();
  CLR_RT_HeapBlock *device_block = (CLR_RT_HeapBlock *)object_device->objBlock;

  setUpdatePending();
  switch (var) {
  case _updatePending: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___isUpdatePending];
    id.NumericByRef().u1 = *(uint32_t *)value;
    break;
  }
  case _identifier: {
    CLR_RT_HeapBlock &id = device_block[Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject::FIELD___objectIdentifier];
    id.NumericByRef().u4 = *(uint32_t *)value;
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
  default: {

    break;
  }
  }
}

uint32_t getDeviceValue(object_deviceValues_t var, void *address) {
  bacObj_Device_t *object_device = getDeviceObject();
  CLR_RT_HeapBlock *device_block = (CLR_RT_HeapBlock *)object_device->objBlock;

  uint32_t value = 0;
  switch (var) {
  case _systemStatus: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___systemStatus].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
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
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___protocolVersion].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
    return 1;
    break;
  }
  case _protocolRevision: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___protocolRevision].NumericByRefConst().u1;
    memcpy(address, (void *)&value, 1);
    return 1;
    break;
  }
  case _vendorId: {
    value = device_block[Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO::FIELD___vendorId].NumericByRefConst().u4;
    memcpy(address, (void *)&value, 4);
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

  setUpdatePending();
  switch (var) {
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
    id.NumericByRef().u4 = *(uint32_t *)value;
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

#pragma GCC pop_options

#ifdef __cplusplus
}
#endif