/*
 * GlobalEventsFlags.c
 *
 *  Created on: 06.08.2019
 *      Author: Jakub Standarski
 */

#include "GlobalEventsFlags.h"




///////////////////////////////////////////////////////////
//  xGlobalEventsFlags defined in GlobalEventsFlags.h    //
//////////////////////////////////////////////////////////
EventGroupHandle_t xGlobalEventsFlags;




/************************************************************************************************************/
/*                                                                                                          */
/*                            GLOBALEVENTSFLAGS PUBLIC FUNCTIONS DEFINITIONS                                */
/*                                                                                                          */
/************************************************************************************************************/

void GlobalEventsFlags_Init(void)
{
    xGlobalEventsFlags = xEventGroupCreate();
}