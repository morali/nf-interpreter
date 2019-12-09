/**************************************************************************
 *
 * Copyright (C) 2006 Steve Karg <skarg@users.sourceforge.net>
 * Copyright (C) 2011 Krzysztof Malorny <malornykrzysztof@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *********************************************************************/

/* Analog Value Objects - customize for your use */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "av.h"
#include "bacapp.h"
#include "bacdcode.h"
#include "bacdef.h"
#include "bacenum.h"
#include "bactext.h"
#include "config.h" /* the custom stuff */
#include "device.h"
#include "handlers.h"

#include "isma_bacnet_objects.h"
#include "isma_bacnet_objects_helper.h"

#include <math.h>

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Analog_Value_Properties_Required[] = {PROP_OBJECT_IDENTIFIER, PROP_OBJECT_NAME,    PROP_OBJECT_TYPE,   PROP_PRESENT_VALUE, PROP_STATUS_FLAGS,
                                                       PROP_EVENT_STATE,       PROP_OUT_OF_SERVICE, PROP_PROPERTY_LIST, PROP_UNITS,         -1};

static const int Analog_Value_Properties_Optional[] = {PROP_DESCRIPTION,
                                                       PROP_PRIORITY_ARRAY,
                                                       PROP_RELINQUISH_DEFAULT,
#ifdef INTRINSIC_REPORTING
                                                       PROP_TIME_DELAY,
                                                       PROP_NOTIFICATION_CLASS,
                                                       PROP_HIGH_LIMIT,
                                                       PROP_LOW_LIMIT,
                                                       PROP_DEADBAND,
                                                       PROP_LIMIT_ENABLE,
                                                       PROP_EVENT_ENABLE,
                                                       PROP_ACKED_TRANSITIONS,
                                                       PROP_NOTIFY_TYPE,
                                                       PROP_EVENT_TIME_STAMPS,
#endif
                                                       -1};

static const int Analog_Value_Properties_Proprietary[] = {-1};

void Analog_Value_Property_Lists(const int **pRequired, const int **pOptional, const int **pProprietary) {
  if (pRequired)
    *pRequired = Analog_Value_Properties_Required;
  if (pOptional)
    *pOptional = Analog_Value_Properties_Optional;
  if (pProprietary)
    *pProprietary = Analog_Value_Properties_Proprietary;

  return;
}

void Analog_Value_Init(void) {
#if defined(INTRINSIC_REPORTING)
  /* Set handler for GetEventInformation function */
  handler_get_event_information_set(OBJECT_ANALOG_VALUE, Analog_Value_Event_Information);
  /* Set handler for AcknowledgeAlarm function */
  handler_alarm_ack_set(OBJECT_ANALOG_VALUE, Analog_Value_Alarm_Ack);
  /* Set handler for GetAlarmSummary Service */
  handler_get_alarm_summary_set(OBJECT_ANALOG_VALUE, Analog_Value_Alarm_Summary);
#endif
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Analog_Value_Valid_Instance(uint32_t object_instance) {
  if (getAnalogByIndex(object_instance) == NULL)
    return false;
  return true;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Analog_Value_Index_To_Instance(uint32_t index) { return index; }

uint32_t Analog_Value_Object_Iterator(uint32_t index) {
  uint32_t __id = 0;

  if (index == 0xFFFFFFFF) {
    bacObj_AV_t *av_instance = getAnalogListHead();
    getAnalogValue(_av_identifier, (void *)&__id, av_instance);
    if (av_instance == NULL)
      return 0;
  } else {
    bacObj_AV_t *av_instance = getAnalogByIndex(index);
    if (av_instance == NULL)
      return 0;
    getAnalogValue(_av_identifier, (void *)&__id, av_instance->next);
  }
  return __id;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
uint32_t Analog_Value_Count(void) {
  uint32_t ret = 0;
  bacObj_AV_t *head = getAnalogListHead();
  while (head != NULL) {
    ret++;
    head = head->next;
  }
  return ret;
}

bool Analog_Value_Name_Set(char *name, uint32_t length, uint32_t object_instance) {
  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  if (av_instance == NULL)
    return false;

  setAnalogValue(_av_name, (void *)name, av_instance);
  // TODO: Utilize length
  (void)length;
  return true;
}

/* note: the object name must be unique within this device */
bool Analog_Value_Object_Name(uint32_t object_instance, BACNET_CHARACTER_STRING *object_name) {
  bool status = false;
  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  if (av_instance == NULL)
    return false;

  const char *__name = NULL;
  getAnalogValue(_av_name, (void *)&__name, av_instance);
  if (__name == NULL)
    return false;
  status = characterstring_init_ansi(object_name, __name);
  return status;
}

/* note: the object name must be unique within this device */
bool Analog_Value_Object_Descr(uint32_t object_instance, BACNET_CHARACTER_STRING *object_name) {
  bool status = false;
  bacObj_AV_t *av_instance = getAnalogByIndex(object_instance);
  if (av_instance == NULL)
    return false;

  const char *__descr = NULL;
  getAnalogValue(_av_description, (void *)&__descr, av_instance);
  if (__descr == NULL)
    return false;
  status = characterstring_init_ansi(object_name, __descr);
  return status;
}

/* return apdu len, or BACNET_STATUS_ERROR on error */
int Analog_Value_Read_Property(BACNET_READ_PROPERTY_DATA *rp_data) {
  int apdu_len = 0; /* return value */

  BACNET_BIT_STRING bit_string;
  BACNET_CHARACTER_STRING char_string;

  bacObj_AV_t *av_instance = NULL;

  float real_value = NAN;
  uint32_t incoming_value = 0;

  bool state = false;
  uint8_t *apdu = NULL;
  int32_t len = 0;
  uint16_t i = 0;

  if ((rp_data == NULL) || (rp_data->application_data == NULL) || (rp_data->application_data_len == 0)) {
    return 0;
  }

  apdu = rp_data->application_data;

  /* Get pointer to AV instance */
  av_instance = getAnalogByIndex(rp_data->object_instance);
  if (av_instance == NULL)
    return BACNET_STATUS_ERROR;

  switch (rp_data->object_property) {
  case PROP_OBJECT_IDENTIFIER:
    apdu_len = encode_application_object_id(&apdu[0], OBJECT_ANALOG_VALUE, rp_data->object_instance);
    break;
  case PROP_OBJECT_NAME:
    Analog_Value_Object_Name(rp_data->object_instance, &char_string);
    apdu_len = encode_application_character_string(&apdu[0], &char_string);
    break;
  case PROP_DESCRIPTION:
    Analog_Value_Object_Descr(rp_data->object_instance, &char_string);
    apdu_len = encode_application_character_string(&apdu[0], &char_string);
    break;
  case PROP_OBJECT_TYPE:
    apdu_len = encode_application_enumerated(&apdu[0], OBJECT_ANALOG_VALUE);
    break;
  case PROP_PRESENT_VALUE:
    real_value = Get_PresentValue(rp_data->object_instance);
    apdu_len = encode_application_real(&apdu[0], real_value);
    break;
  case PROP_STATUS_FLAGS:
    bitstring_init(&bit_string);
#if defined(INTRINSIC_REPORTING)
    bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, av_instance->Event_State ? true : false);
#else
    bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
#endif
    bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
    bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
    bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE, av_instance->outOfService);
    apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
    break;
  case PROP_EVENT_STATE:
#if defined(INTRINSIC_REPORTING)
    apdu_len = encode_application_enumerated(&apdu[0], av_instance->Event_State);
#else
    apdu_len = encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
#endif
    break;
  case PROP_OUT_OF_SERVICE:
    state = av_instance->outOfService;
    apdu_len = encode_application_boolean(&apdu[0], state);
    break;
  case PROP_UNITS:
    getAnalogValue(_units, (void *)&incoming_value, av_instance);
    apdu_len = encode_application_enumerated(&apdu[0], incoming_value);
    break;
  case PROP_PROPERTY_LIST:
    apdu_len = property_list_encode(rp_data, Analog_Value_Properties_Required, Analog_Value_Properties_Optional, Analog_Value_Properties_Proprietary);
    break;
  case PROP_PRIORITY_ARRAY:
    /* Array element zero is the number of elements in the array */
    if (rp_data->array_index == 0)
      apdu_len = encode_application_unsigned(&apdu[0], BACNET_MAX_PRIORITY);
    /* if no index was specified, then try to encode the entire list */
    /* into one packet. */
    else if (rp_data->array_index == BACNET_ARRAY_ALL) {
      float *value = Extract_Float(rp_data->object_instance);
      bool *is_not_null = Extract_Bool(rp_data->object_instance);
      for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
        /* FIXME: check if we have room before adding it to APDU */
        if (!is_not_null[i])
          len = encode_application_null(&apdu[apdu_len]);
        else {
          len = encode_application_real(&apdu[apdu_len], value[i]);
        }
        /* add it if we have room */
        if ((apdu_len + len) < MAX_APDU)
          apdu_len += len;
        else {
          rp_data->error_class = ERROR_CLASS_SERVICES;
          rp_data->error_code = ERROR_CODE_NO_SPACE_FOR_OBJECT;
          apdu_len = BACNET_STATUS_ERROR;
          break;
        }
      }
    } else {
      float *value = Extract_Float(rp_data->object_instance);
      bool *is_not_null = Extract_Bool(rp_data->object_instance);
      if (rp_data->array_index <= BACNET_MAX_PRIORITY) {
        if (!is_not_null[i])
          apdu_len = encode_application_null(&apdu[apdu_len]);
        else {
          apdu_len = encode_application_real(&apdu[apdu_len], value[i]);
        }
      } else {
        rp_data->error_class = ERROR_CLASS_PROPERTY;
        rp_data->error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
        apdu_len = BACNET_STATUS_ERROR;
      }
    }
    break;
  case PROP_RELINQUISH_DEFAULT:
    if (isnan(av_instance->relinquishDefault)) {
      apdu_len = encode_application_null(&apdu[0]);
    } else {
      apdu_len = encode_application_real(&apdu[0], av_instance->relinquishDefault);
    }
    break;
  case PROP_COV_INCREMENT:
    getAnalogValue(_covIncrement, (void *)&incoming_value, av_instance);
    break;
#if defined(INTRINSIC_REPORTING)
  case PROP_TIME_DELAY:
    apdu_len = encode_application_unsigned(&apdu[0], av_instance->Time_Delay);
    break;
  case PROP_NOTIFICATION_CLASS:
    apdu_len = encode_application_unsigned(&apdu[0], av_instance->Notification_Class);
    break;
  case PROP_HIGH_LIMIT:
    apdu_len = encode_application_real(&apdu[0], av_instance->High_Limit);
    break;
  case PROP_LOW_LIMIT:
    apdu_len = encode_application_real(&apdu[0], av_instance->Low_Limit);
    break;
  case PROP_DEADBAND:
    apdu_len = encode_application_real(&apdu[0], av_instance->Deadband);
    break;
  case PROP_LIMIT_ENABLE:
    bitstring_init(&bit_string);
    bitstring_set_bit(&bit_string, 0, (av_instance->Limit_Enable & EVENT_LOW_LIMIT_ENABLE) ? true : false);
    bitstring_set_bit(&bit_string, 1, (av_instance->Limit_Enable & EVENT_HIGH_LIMIT_ENABLE) ? true : false);
    apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
    break;
  case PROP_EVENT_ENABLE:
    bitstring_init(&bit_string);
    bitstring_set_bit(&bit_string, TRANSITION_TO_OFFNORMAL, (av_instance->Event_Enable & EVENT_ENABLE_TO_OFFNORMAL) ? true : false);
    bitstring_set_bit(&bit_string, TRANSITION_TO_FAULT, (av_instance->Event_Enable & EVENT_ENABLE_TO_FAULT) ? true : false);
    bitstring_set_bit(&bit_string, TRANSITION_TO_NORMAL, (av_instance->Event_Enable & EVENT_ENABLE_TO_NORMAL) ? true : false);
    apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
    break;
  case PROP_ACKED_TRANSITIONS:
    bitstring_init(&bit_string);
    bitstring_set_bit(&bit_string, TRANSITION_TO_OFFNORMAL, av_instance->Acked_Transitions[TRANSITION_TO_OFFNORMAL].bIsAcked);
    bitstring_set_bit(&bit_string, TRANSITION_TO_FAULT, av_instance->Acked_Transitions[TRANSITION_TO_FAULT].bIsAcked);
    bitstring_set_bit(&bit_string, TRANSITION_TO_NORMAL, av_instance->Acked_Transitions[TRANSITION_TO_NORMAL].bIsAcked);
    apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
    break;
  case PROP_NOTIFY_TYPE:
    apdu_len = encode_application_enumerated(&apdu[0], av_instance->Notify_Type ? NOTIFY_EVENT : NOTIFY_ALARM);
    break;
  case PROP_EVENT_TIME_STAMPS:
    /* Array element zero is the number of elements in the array */
    if (rp_data->array_index == 0)
      apdu_len = encode_application_unsigned(&apdu[0], MAX_BACNET_EVENT_TRANSITION);
    /* if no index was specified, then try to encode the entire list */
    /* into one packet. */
    else if (rp_data->array_index == BACNET_ARRAY_ALL) {
      for (i = 0; i < MAX_BACNET_EVENT_TRANSITION; i++) {
        len = encode_opening_tag(&apdu[apdu_len], TIME_STAMP_DATETIME);
        len += encode_application_date(&apdu[apdu_len + len], &av_instance->Event_Time_Stamps[i].date);
        len += encode_application_time(&apdu[apdu_len + len], &av_instance->Event_Time_Stamps[i].time);
        len += encode_closing_tag(&apdu[apdu_len + len], TIME_STAMP_DATETIME);
        /* add it if we have room */
        if ((apdu_len + len) < MAX_APDU)
          apdu_len += len;
        else {
          rp_data->error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
          apdu_len = BACNET_STATUS_ABORT;
          break;
        }
      }
    } else if (rp_data->array_index <= MAX_BACNET_EVENT_TRANSITION) {
      apdu_len = encode_opening_tag(&apdu[apdu_len], TIME_STAMP_DATETIME);
      apdu_len += encode_application_date(&apdu[apdu_len], &av_instance->Event_Time_Stamps[rp_data->array_index - 1].date);
      apdu_len += encode_application_time(&apdu[apdu_len], &av_instance->Event_Time_Stamps[rp_data->array_index - 1].time);
      apdu_len += encode_closing_tag(&apdu[apdu_len], TIME_STAMP_DATETIME);
    } else {
      rp_data->error_class = ERROR_CLASS_PROPERTY;
      rp_data->error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
      apdu_len = BACNET_STATUS_ERROR;
    }
    break;
#endif

  default:
    rp_data->error_class = ERROR_CLASS_PROPERTY;
    rp_data->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
    apdu_len = BACNET_STATUS_ERROR;
    break;
  }
  /*  only array properties can have array options */
  if ((apdu_len >= 0) && (rp_data->object_property != PROP_PRIORITY_ARRAY) && (rp_data->object_property != PROP_EVENT_TIME_STAMPS) && (rp_data->object_property != PROP_PROPERTY_LIST) &&
      (rp_data->array_index != BACNET_ARRAY_ALL)) {
    rp_data->error_class = ERROR_CLASS_PROPERTY;
    rp_data->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
    apdu_len = BACNET_STATUS_ERROR;
  }
  return apdu_len;
}

/* returns true if successful */
bool Analog_Value_Write_Property(BACNET_WRITE_PROPERTY_DATA *wp_data) {
  bool status = false; /* return value */
  int len = 0;

  BACNET_APPLICATION_DATA_VALUE value;

  /* decode the some of the request */
  len = bacapp_decode_application_data(wp_data->application_data, wp_data->application_data_len, &value);
  /* FIXME: len < application_data_len: more data? */
  if (len < 0) {
    /* error while decoding - a value larger than we can handle */
    wp_data->error_class = ERROR_CLASS_PROPERTY;
    wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
    return false;
  }
  if ((wp_data->object_property != PROP_PRIORITY_ARRAY) && (wp_data->object_property != PROP_PROPERTY_LIST) && (wp_data->object_property != PROP_EVENT_TIME_STAMPS) &&
      (wp_data->array_index != BACNET_ARRAY_ALL)) {
    /*  only array properties can have array options */
    wp_data->error_class = ERROR_CLASS_PROPERTY;
    wp_data->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
    return false;
  }

  bacObj_AV_t *av_instance = getAnalogByIndex(wp_data->object_instance);
  if (av_instance == NULL)
    return BACNET_STATUS_ERROR;

  switch (wp_data->object_property) {
  case PROP_PRESENT_VALUE:
    // wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    // break;
    /* Disabled option for writing present value */
    if (value.tag == BACNET_APPLICATION_TAG_REAL) {
      /* Command priority 6 is reserved for use by Minimum On/Off
         algorithm and may not be used for other purposes in any
         object. */
      if (Set_AnalogValue(wp_data->object_instance, value.type.Real, true, wp_data->priority)) {
        status = true;
      } else if (wp_data->priority == 6) {
        /* Command priority 6 is reserved for use by Minimum On/Off
           algorithm and may not be used for other purposes in any
           object. */
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
      } else {
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
      }
    } else if (value.tag == BACNET_APPLICATION_TAG_NULL) {
      /* Command priority 6 is reserved for use by Minimum On/Off
                         algorithm and may not be used for other purposes in any
                         object. */
      if (Set_AnalogValue(wp_data->object_instance, NAN, false, wp_data->priority)) {
        status = true;
      } else if (wp_data->priority == 6) {
        /* Command priority 6 is reserved for use by Minimum On/Off
           algorithm and may not be used for other purposes in any
           object. */
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
      } else {
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
      }
    } else {
      status = false;
      wp_data->error_class = ERROR_CLASS_PROPERTY;
      wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
    }
    break;
  case PROP_RELINQUISH_DEFAULT:
    if (value.tag != BACNET_APPLICATION_TAG_NULL && !isnan(value.type.Real)) {
      av_instance->relinquishDefault = value.type.Real;
      status = true;
    } else {
      wp_data->error_class = ERROR_CLASS_PROPERTY;
      wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
    }
    break;
  case PROP_OUT_OF_SERVICE:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BOOLEAN, &wp_data->error_class, &wp_data->error_code);
    if (status) {
      av_instance->outOfService = value.type.Boolean;
    }
    break;
  case PROP_UNITS:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
    /* Disabled option for writing present value */
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_ENUMERATED, &wp_data->error_class, &wp_data->error_code);
    if (status) {
      setAnalogValue(_units, (void *)&value.type.Enumerated, av_instance);
    }
    break;
#if defined(INTRINSIC_REPORTING)
  case PROP_TIME_DELAY:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_UNSIGNED_INT, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      av_instance->Time_Delay = value.type.Unsigned_Int;
      av_instance->Remaining_Time_Delay = av_instance->Time_Delay;
    }
    break;
  case PROP_NOTIFICATION_CLASS:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_UNSIGNED_INT, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      av_instance->Notification_Class = value.type.Unsigned_Int;
    }
    break;
  case PROP_HIGH_LIMIT:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_REAL, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      av_instance->High_Limit = value.type.Real;
    }
    break;
  case PROP_LOW_LIMIT:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_REAL, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      av_instance->Low_Limit = value.type.Real;
    }
    break;
  case PROP_DEADBAND:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_REAL, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      av_instance->Deadband = value.type.Real;
    }
    break;
  case PROP_LIMIT_ENABLE:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BIT_STRING, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      if (value.type.Bit_String.bits_used == 2) {
        av_instance->Limit_Enable = value.type.Bit_String.value[0];
      } else {
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
        status = false;
      }
    }
    break;
  case PROP_EVENT_ENABLE:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BIT_STRING, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      if (value.type.Bit_String.bits_used == 3) {
        av_instance->Event_Enable = value.type.Bit_String.value[0];
      } else {
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
        status = false;
      }
    }
    break;
  case PROP_NOTIFY_TYPE:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_ENUMERATED, &wp_data->error_class, &wp_data->error_code);

    if (status) {
      switch ((BACNET_NOTIFY_TYPE)value.type.Enumerated) {
      case NOTIFY_EVENT:
        av_instance->Notify_Type = 1;
        break;
      case NOTIFY_ALARM:
        av_instance->Notify_Type = 0;
        break;
      default:
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
        status = false;
        break;
      }
    }
    break;
#endif
  case PROP_OBJECT_IDENTIFIER:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
    /* Disabled option for writing object identifier */
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_OBJECT_ID, &wp_data->error_class, &wp_data->error_code);
    setAnalogValue(_av_identifier, (void *)&value.type.Object_Id.instance, av_instance);
    break;
  case PROP_OBJECT_NAME:
    status = WPValidateString(&value, MAX_DEV_MOD_LEN, true, &wp_data->error_class, &wp_data->error_code);
    if (status) {
      Analog_Value_Name_Set(characterstring_value(&value.type.Character_String), characterstring_length(&value.type.Character_String), wp_data->object_instance);
    }
    break;
  case PROP_COV_INCREMENT:
    setAnalogValue(_covIncrement, (void *)&value.type.Unsigned_Int, av_instance);
    break;
  case PROP_OBJECT_TYPE:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
  case PROP_STATUS_FLAGS:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
  case PROP_EVENT_STATE:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
  case PROP_PROPERTY_LIST:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
  case PROP_DESCRIPTION:
    status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_CHARACTER_STRING, &wp_data->error_class, &wp_data->error_code);
    setAnalogValue(_av_description, (void *)value.type.Character_String.value, av_instance);
    break;
  case PROP_PRIORITY_ARRAY:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
#if defined(INTRINSIC_REPORTING)
  case PROP_ACKED_TRANSITIONS:
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
  case PROP_EVENT_TIME_STAMPS:
#endif
    wp_data->error_class = ERROR_CLASS_PROPERTY;
    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
    break;
  default:
    wp_data->error_class = ERROR_CLASS_PROPERTY;
    wp_data->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
    break;
  }
  return status;
}

void Analog_Value_Intrinsic_Reporting(uint32_t object_instance) {
  (void)object_instance;
#if defined(INTRINSIC_REPORTING)
  BACNET_EVENT_NOTIFICATION_DATA event_data;
  BACNET_CHARACTER_STRING msgText;
  uint8_t FromState = 0;
  uint8_t ToState;
  float ExceededLimit = 0.0f;
  float PresentVal = 0.0f;
  bool SendNotify = false;

  bacObj_AV_t *av_instance = getAnalogByIndex(wp_data->object_instance);
  if (av_instance == NULL)
    return BACNET_STATUS_ERROR;

  /* check limits */
  if (!av_instance->Limit_Enable)
    return; /* limits are not configured */

  if (av_instance->Ack_notify_data.bSendAckNotify) {
    /* clean bSendAckNotify flag */
    av_instance->Ack_notify_data.bSendAckNotify = false;
    /* copy toState */
    ToState = av_instance->Ack_notify_data.EventState;

#if PRINT_ENABLED
    fprintf(stderr, "Send Acknotification for (%s,%d).\n", bactext_object_type_name(OBJECT_ANALOG_VALUE), object_instance);
#endif /* PRINT_ENABLED */

    characterstring_init_ansi(&msgText, "AckNotification");

    /* Notify Type */
    event_data.notifyType = NOTIFY_ACK_NOTIFICATION;

    /* Send EventNotification. */
    SendNotify = true;
  } else {
    /* actual Present_Value */
    PresentVal = Get_PresentValue(object_instance);
    FromState = av_instance->Event_State;
    switch (av_instance->Event_State) {
    case EVENT_STATE_NORMAL:
      /* A TO-OFFNORMAL event is generated under these conditions:
         (a) the Present_Value must exceed the High_Limit for a minimum
         period of time, specified in the Time_Delay property, and
         (b) the HighLimitEnable flag must be set in the Limit_Enable property, and
         (c) the TO-OFFNORMAL flag must be set in the Event_Enable property. */
      if ((PresentVal > av_instance->High_Limit) && ((av_instance->Limit_Enable & EVENT_HIGH_LIMIT_ENABLE) == EVENT_HIGH_LIMIT_ENABLE) &&
          ((av_instance->Event_Enable & EVENT_ENABLE_TO_OFFNORMAL) == EVENT_ENABLE_TO_OFFNORMAL)) {
        if (!av_instance->Remaining_Time_Delay)
          av_instance->Event_State = EVENT_STATE_HIGH_LIMIT;
        else
          av_instance->Remaining_Time_Delay--;
        break;
      }

      /* A TO-OFFNORMAL event is generated under these conditions:
         (a) the Present_Value must exceed the Low_Limit plus the Deadband
         for a minimum period of time, specified in the Time_Delay property, and
         (b) the LowLimitEnable flag must be set in the Limit_Enable property, and
         (c) the TO-NORMAL flag must be set in the Event_Enable property. */
      if ((PresentVal < av_instance->Low_Limit) && ((av_instance->Limit_Enable & EVENT_LOW_LIMIT_ENABLE) == EVENT_LOW_LIMIT_ENABLE) &&
          ((av_instance->Event_Enable & EVENT_ENABLE_TO_OFFNORMAL) == EVENT_ENABLE_TO_OFFNORMAL)) {
        if (!av_instance->Remaining_Time_Delay)
          av_instance->Event_State = EVENT_STATE_LOW_LIMIT;
        else
          av_instance->Remaining_Time_Delay--;
        break;
      }
      /* value of the object is still in the same event state */
      av_instance->Remaining_Time_Delay = av_instance->Time_Delay;
      break;

    case EVENT_STATE_HIGH_LIMIT:
      /* Once exceeded, the Present_Value must fall below the High_Limit minus
         the Deadband before a TO-NORMAL event is generated under these conditions:
         (a) the Present_Value must fall below the High_Limit minus the Deadband
         for a minimum period of time, specified in the Time_Delay property, and
         (b) the HighLimitEnable flag must be set in the Limit_Enable property, and
         (c) the TO-NORMAL flag must be set in the Event_Enable property. */
      if ((PresentVal < av_instance->High_Limit - av_instance->Deadband) && ((av_instance->Limit_Enable & EVENT_HIGH_LIMIT_ENABLE) == EVENT_HIGH_LIMIT_ENABLE) &&
          ((av_instance->Event_Enable & EVENT_ENABLE_TO_NORMAL) == EVENT_ENABLE_TO_NORMAL)) {
        if (!av_instance->Remaining_Time_Delay)
          av_instance->Event_State = EVENT_STATE_NORMAL;
        else
          av_instance->Remaining_Time_Delay--;
        break;
      }
      /* value of the object is still in the same event state */
      av_instance->Remaining_Time_Delay = av_instance->Time_Delay;
      break;

    case EVENT_STATE_LOW_LIMIT:
      /* Once the Present_Value has fallen below the Low_Limit,
         the Present_Value must exceed the Low_Limit plus the Deadband
         before a TO-NORMAL event is generated under these conditions:
         (a) the Present_Value must exceed the Low_Limit plus the Deadband
         for a minimum period of time, specified in the Time_Delay property, and
         (b) the LowLimitEnable flag must be set in the Limit_Enable property, and
         (c) the TO-NORMAL flag must be set in the Event_Enable property. */
      if ((PresentVal > av_instance->Low_Limit + av_instance->Deadband) && ((av_instance->Limit_Enable & EVENT_LOW_LIMIT_ENABLE) == EVENT_LOW_LIMIT_ENABLE) &&
          ((av_instance->Event_Enable & EVENT_ENABLE_TO_NORMAL) == EVENT_ENABLE_TO_NORMAL)) {
        if (!av_instance->Remaining_Time_Delay)
          av_instance->Event_State = EVENT_STATE_NORMAL;
        else
          av_instance->Remaining_Time_Delay--;
        break;
      }
      /* value of the object is still in the same event state */
      av_instance->Remaining_Time_Delay = av_instance->Time_Delay;
      break;

    default:
      return; /* shouldn't happen */
    }         /* switch (FromState) */

    ToState = av_instance->Event_State;

    if (FromState != ToState) {
      /* Event_State has changed.
         Need to fill only the basic parameters of this type of event.
         Other parameters will be filled in common function. */

      switch (ToState) {
      case EVENT_STATE_HIGH_LIMIT:
        ExceededLimit = av_instance->High_Limit;
        characterstring_init_ansi(&msgText, av_instance->HighLimitText);
        break;

      case EVENT_STATE_LOW_LIMIT:
        ExceededLimit = av_instance->Low_Limit;
        characterstring_init_ansi(&msgText, av_instance->LowLimitText);
        break;

      case EVENT_STATE_NORMAL:
        if (FromState == EVENT_STATE_HIGH_LIMIT) {
          ExceededLimit = av_instance->High_Limit;
          characterstring_init_ansi(&msgText, av_instance->ToNormalText);
        } else {
          ExceededLimit = av_instance->Low_Limit;
          characterstring_init_ansi(&msgText, av_instance->ToNormalText);
        }
        break;

      default:
        ExceededLimit = 0;
        break;
      } /* switch (ToState) */

#if PRINT_ENABLED
      fprintf(stderr, "Event_State for (%s,%d) goes from %s to %s.\n", bactext_object_type_name(OBJECT_ANALOG_VALUE), object_instance, bactext_event_state_name(FromState),
              bactext_event_state_name(ToState));
#endif /* PRINT_ENABLED */

      /* Notify Type */
      event_data.notifyType = av_instance->Notify_Type;

      /* Send EventNotification. */
      SendNotify = true;
    }
  }

  if (SendNotify) {
    /* Event Object Identifier */
    event_data.eventObjectIdentifier.type = OBJECT_ANALOG_VALUE;
    event_data.eventObjectIdentifier.instance = object_instance;

    /* Time Stamp */
    event_data.timeStamp.tag = TIME_STAMP_DATETIME;
    Device_getCurrentDateTime(&event_data.timeStamp.value.dateTime);

    if (event_data.notifyType != NOTIFY_ACK_NOTIFICATION) {
      /* fill Event_Time_Stamps */
      switch (ToState) {
      case EVENT_STATE_HIGH_LIMIT:
      case EVENT_STATE_LOW_LIMIT:
        av_instance->Event_Time_Stamps[TRANSITION_TO_OFFNORMAL] = event_data.timeStamp.value.dateTime;
        break;

      case EVENT_STATE_FAULT:
        av_instance->Event_Time_Stamps[TRANSITION_TO_FAULT] = event_data.timeStamp.value.dateTime;
        break;

      case EVENT_STATE_NORMAL:
        av_instance->Event_Time_Stamps[TRANSITION_TO_NORMAL] = event_data.timeStamp.value.dateTime;
        break;
      }
    }

    /* Notification Class */
    event_data.notificationClass = av_instance->Notification_Class;

    /* Event Type */
    event_data.eventType = EVENT_OUT_OF_RANGE;

    /* Message Text */
    event_data.messageText = &msgText;

    /* Notify Type */
    /* filled before */

    /* From State */
    if (event_data.notifyType != NOTIFY_ACK_NOTIFICATION)
      event_data.fromState = FromState;

    /* To State */
    event_data.toState = av_instance->Event_State;

    /* Event Values */
    if (event_data.notifyType != NOTIFY_ACK_NOTIFICATION) {
      /* Value that exceeded a limit. */
      event_data.notificationParams.outOfRange.exceedingValue = PresentVal;
      /* Status_Flags of the referenced object. */
      bitstring_init(&event_data.notificationParams.outOfRange.statusFlags);
      bitstring_set_bit(&event_data.notificationParams.outOfRange.statusFlags, STATUS_FLAG_IN_ALARM, av_instance->Event_State ? true : false);
      bitstring_set_bit(&event_data.notificationParams.outOfRange.statusFlags, STATUS_FLAG_FAULT, false);
      bitstring_set_bit(&event_data.notificationParams.outOfRange.statusFlags, STATUS_FLAG_OVERRIDDEN, false);
      bitstring_set_bit(&event_data.notificationParams.outOfRange.statusFlags, STATUS_FLAG_OUT_OF_SERVICE, av_instance->outOfService);
      /* Deadband used for limit checking. */
      event_data.notificationParams.outOfRange.deadband = av_instance->Deadband;
      /* Limit that was exceeded. */
      event_data.notificationParams.outOfRange.exceededLimit = ExceededLimit;
    }

    /* add data from notification class */
    Notification_Class_common_reporting_function(network, &event_data);

    /* Ack required */
    if ((event_data.notifyType != NOTIFY_ACK_NOTIFICATION) && (event_data.ackRequired == true)) {
      switch (event_data.toState) {
      case EVENT_STATE_OFFNORMAL:
      case EVENT_STATE_HIGH_LIMIT:
      case EVENT_STATE_LOW_LIMIT:
        av_instance->Acked_Transitions[TRANSITION_TO_OFFNORMAL].bIsAcked = false;
        av_instance->Acked_Transitions[TRANSITION_TO_OFFNORMAL].Time_Stamp = event_data.timeStamp.value.dateTime;
        break;

      case EVENT_STATE_FAULT:
        av_instance->Acked_Transitions[TRANSITION_TO_FAULT].bIsAcked = false;
        av_instance->Acked_Transitions[TRANSITION_TO_FAULT].Time_Stamp = event_data.timeStamp.value.dateTime;
        break;

      case EVENT_STATE_NORMAL:
        av_instance->Acked_Transitions[TRANSITION_TO_NORMAL].bIsAcked = false;
        av_instance->Acked_Transitions[TRANSITION_TO_NORMAL].Time_Stamp = event_data.timeStamp.value.dateTime;
        break;
      }
    }
  }
#endif /* defined(INTRINSIC_REPORTING) */
}
