/*
 * GlobalEventsFlags.h
 *
 *  Created on: 06.08.2019
 *      Author: Jakub Standarski
 */

#ifndef GLOBALEVENTSFLAGS_H_
#define GLOBALEVENTSFLAGS_H_

#ifdef __cplusplus
extern "C" {
#endif

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
 *                                Bit 1  -> left for RTC
 *                                Bit 2  -> SPI2 transfer procedure - Dipswitches and LEDs (0 - not done, 1 - done)
 *                                Bit 3  ->
 *                                Bit 4  ->
 *                                Bit 5  ->
 *                                Bit 6  ->
 *                                Bit 7  ->
 *                                Bit 8  ->
 *                                ...
 */




/************************************************************************************************************/
/*                                                                                                          */
/*                                     GLOBALEVENTSFLAGS DEFINES                                            */
/*                                                                                                          */
/************************************************************************************************************/

/* Generic defines */
#define NOT_SET     0
#define SET         1

/* Bits position defines */
#define MAC_ADDRESS_READ        (1 << 0)
#define SPI2_TRANSFER_DONE      (1 << 2)




/************************************************************************************************************/
/*                                                                                                          */
/*                            GLOBALEVENTSFLAGS PUBLIC FUNCTIONS DECLARATIONS                               */
/*                                                                                                          */
/************************************************************************************************************/

void GlobalEventsFlags_Init(void);




#ifdef __cplusplus
}
#endif

#endif /* GLOBALEVENTSFLAGS_H_ */