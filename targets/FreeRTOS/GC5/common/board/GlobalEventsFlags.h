/*
 * GlobalEventsFlags.h
 *
 *  Created on: 06.08.2019
 *      Author: Jakub Standarski
 */

#ifndef GLOBALEVENTSFLAGS_H_
#define GLOBALEVENTSFLAGS_H_

#include "FreeRTOS.h"
#include "event_groups.h"

/////////////////////////////////////////////////////////
//   GlobalEventsFlags global variable declarations    //
/////////////////////////////////////////////////////////
extern EventGroupHandle_t xGlobalEventsFlags;
/*
 *  if    configUSE_16_BIT_TICKS is set to 1, then number of bits implemented within an event group is 8
 *  else  configUSE_16_BIT_TICKS is set to 0, then number of bits implemented within an event group is 24
 *
 *  Bits positions definition:    Bit 0  -> MAC Address read procedure (0 - not read, 1 - read)
 *                                Bit 1  ->
 *                                Bit 2  ->
 *                                Bit 3  ->
 *                                Bit 4  ->
 *                                Bit 5  ->
 *                                Bit 6  ->
 *                                Bit 7  ->
 *                                Bit 8  ->
 *                                ...
 */

/* Bits position defines */
#define MAC_ADDRESS_READ (1 << 0)
#define EVENT_ETH_OK (1 << 1)

void GlobalEventsFlags_Init(void);

#endif /* GLOBALEVENTSFLAGS_H_ */