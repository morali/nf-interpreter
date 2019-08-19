/*
 * External_RTC.h
 *
 *  Created on: 01.08.2019
 *      Author: Jakub Standarski
 */

#ifndef EXTERNAL_RTC_H_
#define EXTERNAL_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "i2c.h"
#include "task.h"
#include "time.h"




/************************************************************************************************************/
/*                                                                                                          */
/*                                  RTC PUBLIC FUNCTIONS DECLARATIONS                                       */
/*                                                                                                          */
/************************************************************************************************************/

struct tm *RTC_ReadTime();

void RTC_SetTime(struct tm *rtcTime);

void vRtcThread(void *pvParameters);




#ifdef __cplusplus
}
#endif

#endif /* EXTERNAL_RTC_H_ */