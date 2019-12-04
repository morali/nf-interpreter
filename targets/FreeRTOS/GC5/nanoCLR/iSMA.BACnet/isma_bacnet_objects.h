#ifndef _ISMA_BACNET_OBJECTS_H_
#define _ISMA_BACNET_OBJECTS_H_

#include <stdint.h>

#include "nc.h"
#include "datetime.h"
#include "bacenum.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bacObj_Device {
  uint32_t udpPort;

  void *objBlock; /* Device PBO defined in isma bacnet C# */
  void *gc;
} bacObj_Device_t;

typedef struct bacObj_AV {
  void *objBlock;
  void *gc;

  uint32_t outOfService;
  float relinquishDefault;

  unsigned Event_State;
#if defined(INTRINSIC_REPORTING)
  uint32_t Time_Delay;
  uint32_t Notification_Class;
  float High_Limit;
  float Low_Limit;
  float Deadband;
  unsigned Limit_Enable;
  unsigned Event_Enable;
  unsigned Notify_Type;

  /* time to generate event notification */
  uint32_t Remaining_Time_Delay;

  char *HighLimitText;
  char *LowLimitText;
  char *ToNormalText;

  /* AckNotification informations */
  ACK_NOTIFICATION Ack_notify_data;

  ACKED_INFO *pAcked_Transitions[MAX_BACNET_EVENT_TRANSITION];
  ACKED_INFO Acked_Transitions[MAX_BACNET_EVENT_TRANSITION];
  BACNET_DATE_TIME *pEvent_Time_Stamps[MAX_BACNET_EVENT_TRANSITION];
  BACNET_DATE_TIME Event_Time_Stamps[MAX_BACNET_EVENT_TRANSITION];
#endif

  struct bacObj_AV *next;
  struct bacObj_AV *prev;
} bacObj_AV_t;

bacObj_Device_t* getDeviceObject();
bacObj_AV_t *getAnalogListHead();

bacObj_AV_t *getAnalogByIndex(uint32_t index);

#ifdef __cplusplus
}
#endif

#endif //_ISMA_BACNET_OBJECTS_H_
