
#ifndef _ISMA_BACNET_OBJECTS_HELPER_H_
#define _ISMA_BACNET_OBJECTS_HELPER_H_

#include <stdint.h>

#include "isma_bacnet_objects.h"

#define MAX_OBJECT_NAME 128

typedef enum object_deviceValues {
  // NOT USED INT HERE _updatePending = 1,           /* int */
  _identifier,                  /* int */
  _name,                        /* string */
  _type,                        /* int (bacnet enum) */
  _systemStatus,                /* int */
  _vendorName,                  /* string */
  _modelName,                   /* string */
  _firmwareRevision,            /* string */
  _applicationSoftwareRevision, /* string */
  _location,                    /* string */
  _device_description,          /* string */
  _protocolVersion,             /* int */
  _protocolRevision,            /* int */
  _vendorId,                    /* uint */
  _apduTimeout,                 /* uint */
  _apduRetries                  /* uint */
} object_deviceValues_t;

typedef enum object_analogValues {
  // NOT USED INT HERE _av_updatePending = 1, /* int */
  _av_identifier, /* int */
  _av_name,       /* string */
  _av_type,
  _presentValue,
  _av_description,
  _statusFlags,
  _reliability,
  _units,
  _covIncrement,
  _presentPriority
} object_analogValues_t;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t Get_DeviceValue(object_deviceValues_t var, void *value);
void Set_DeviceValue(object_deviceValues_t var, void *value);
uint32_t Get_AnalogValue(object_analogValues_t var, void *address, bacObj_AV_t *object_analog);
void Set_AnalogValue(object_analogValues_t var, void *address, bacObj_AV_t *object_analog);
bool Set_AnalogValue_WithPriority(bacObj_AV_t * av_instance, float incoming_float, bool incoming_bool, uint8_t priority);
float Get_PresentValue(uint32_t object_instance);

float *Extract_Float(bacObj_AV_t *av_instance);
bool *Extract_Bool(bacObj_AV_t *av_instance);

#ifdef __cplusplus
}
#endif

#endif //_ISMA_BACNET_OBJECTS_HELPER_H_