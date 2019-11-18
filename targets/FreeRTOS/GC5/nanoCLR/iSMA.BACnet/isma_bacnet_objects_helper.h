
#ifndef _ISMA_BACNET_OBJECTS_HELPER_H_
#define _ISMA_BACNET_OBJECTS_HELPER_H_

#include <stdint.h>

#define MAX_OBJECT_NAME 128

typedef enum object_baseValues {
  _updatePending = 1, /* int */
  _identifier,        /* int */
  _name,              /* string */
  _type               /* int (bacnet enum) */
} object_baseValues_t;

typedef enum object_deviceValues {
  _systemStatus = 5,            /* int */
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
  _presentValue = 5,
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

uint32_t getBaseValue(object_baseValues_t var, void *address);
void setBaseValue(object_baseValues_t var, void *address);
uint32_t getDeviceValue(object_deviceValues_t var, void *value);
void setDeviceValue(object_deviceValues_t var, void *value);
#ifdef __cplusplus
}
#endif

#endif //_ISMA_BACNET_OBJECTS_HELPER_H_