/*
 * MAC_address.h
 *
 *  Created on: 02.08.2019
 *      Author: Jakub Standarski
 */

#ifndef MAC_ADDRESS_H_
#define MAC_ADDRESS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "i2c.h"
#include "task.h"
#include "time.h"
#include "GlobalEventsFlags.h"




/************************************************************************************************************/
/*                                                                                                          */
/*                               MAC ADDRESS PUBLIC FUNCTIONS DECLARATIONS                                  */
/*                                                                                                          */
/************************************************************************************************************/

uint8_t *MAC_GetAddress();

/* FreeRTOS task declaration */
void vMacAddressThread(void *pvParameters);




#ifdef __cplusplus
}
#endif

#endif /* MAC_ADDRESS_H_ */