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
#ifndef AV_H
#define AV_H

#include "rp.h"
#include "wp.h"

#include "isma_bacnet_objects_helper.h"
#include "isma_bacnet_objects.h"

#if defined(INTRINSIC_REPORTING)
#include "alarm_ack.h"
#include "get_alarm_sum.h"
#include "getevent.h"
#include "nc.h"
#endif

void Analog_Value_Init(void);

void Analog_Value_Property_Lists(const int **pRequired, const int **pOptional, const int **pProprietary);
bool Analog_Value_Valid_Instance(uint32_t object_instance);

uint32_t Analog_Value_Count(void);
uint32_t Analog_Value_Index_To_Instance(uint32_t index);
uint32_t Analog_Value_Object_Iterator(uint32_t index);

bool Analog_Value_Object_Name(uint32_t object_instance, BACNET_CHARACTER_STRING *object_name);
bool Analog_Value_Name_Set(char *object_name, uint32_t length, uint32_t object_instance);

int Analog_Value_Read_Property(BACNET_READ_PROPERTY_DATA *rp_data);
bool Analog_Value_Write_Property(BACNET_WRITE_PROPERTY_DATA *wp_data);

/* note: header of Intrinsic_Reporting function is required
   even when INTRINSIC_REPORTING is not defined */
void Analog_Value_Intrinsic_Reporting(uint32_t object_instance);

#if defined(INTRINSIC_REPORTING)
int Analog_Value_Event_Information(unsigned index, BACNET_GET_EVENT_INFORMATION_DATA *getevent_data);
int Analog_Value_Alarm_Ack(BACNET_ALARM_ACK_DATA *alarmack_data, BACNET_ERROR_CODE *error_code);
int Analog_Value_Alarm_Summary(unsigned index, BACNET_GET_ALARM_SUMMARY_DATA *getalarm_data);
#endif

#endif //_AV_H
