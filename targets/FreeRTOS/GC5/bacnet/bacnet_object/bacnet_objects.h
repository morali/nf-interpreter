#ifndef BACNET_OBJECTS_H_
#define BACNET_OBJECTS_H_

#include "bacenum.h"
#include "datetime.h"
#include "nc.h"
#include "stdbool.h"
#include <stddef.h>
#include <string.h>

#include <stdlib.h>

#define MAX_ANALOG_POINTS 200
#define MAX_BINARY_POINTS 200
#define MAX_MSV_POINTS 200
#define MAX_SCHEDULE_POINTS 30
#define MAX_CALENDAR_POINTS 5
#define MAX_HISTORICAL_POINTS 100
#define MAX_NC_POINTS 10

typedef enum { EMPTY, READY_TO_SEND, WAITING_FOR_ADDRESS, WAITING_FOR_ANSWER, TIMEOUT, OK, READ_ERROR, WRITE_ERROR, REJECTED } bacnetTaskStatus_t;

typedef struct BACnetAV {
  char *name;
  char *descr;
  uint32_t units;
  uint32_t outOfService;
  uint32_t id;
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
  float in[16];
} BACnetAV_t;

typedef struct BACnetBV {
  char *name;
  char *descr;
  uint32_t outOfService;
  uint32_t id;
  BACNET_BINARY_PV relinquishDefault;
  char dummy1;
  char dummy2;
  char dummy3;
  unsigned Event_State;
#if defined(INTRINSIC_REPORTING)
  uint32_t Time_Delay;
  uint32_t Notification_Class;
  BACNET_BINARY_PV Alarm_Value;
  char dummy4;
  char dummy5;
  char dummy6;
  unsigned Event_Enable;
  unsigned Notify_Type;

  /* time to generate event notification */
  uint32_t Remaining_Time_Delay;

  char *ToOffNormalText;
  char *ToNormalText;

  /* AckNotification informations */
  ACK_NOTIFICATION Ack_notify_data;

  ACKED_INFO *pAcked_Transitions[MAX_BACNET_EVENT_TRANSITION];
  ACKED_INFO Acked_Transitions[MAX_BACNET_EVENT_TRANSITION];
  BACNET_DATE_TIME *pEvent_Time_Stamps[MAX_BACNET_EVENT_TRANSITION];
  BACNET_DATE_TIME Event_Time_Stamps[MAX_BACNET_EVENT_TRANSITION];
#endif
  BACNET_BINARY_PV in[16];
} BACnetBV_t;

typedef struct ListElement {
  struct ListElement *next;
  struct ListElement *prev;
  void *object;

} ListElement_t;

typedef enum { av = 0, bv } BACtype_t;

BACnetObj_t *B_Initialize();
BACnetObj_t *B_RetHead(BACtype_t type);

void Initialize(BACtype type, void **bacptr);
uint8_t AddObject(BACtype type);
uint32_t Size(BACtype type, void *bacptr);
bool Exist(BACtype type, void *bacptr, void *val);
uint32_t FindId(BACtype type, uint32_t object_instance);
void *FindIdRet(BACtype type, uint32_t object_instance);
void LinkAtFront();
void *retObj(BACtype type);
void **retObj_p(BACtype type);

#endif /* BACNET_OBJECTS_H */