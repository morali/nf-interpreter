/*
 * localIO_Timers.c
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "LocalIO_Timers.h"



/************************************************************************************************************/
/*                                                                                                          */
/*                                           PRIVATE DEFINES                                                */
/*                                                                                                          */
/************************************************************************************************************/






/************************************************************************************************************/
/*                                                                                                          */
/*                                          PRIVATE VARIABLES                                               */
/*                                                                                                          */
/************************************************************************************************************/








/************************************************************************************************************/
/*                                                                                                          */
/*                              LOCALIO_TIMERS PUBLIC FUNCTIONS DEFINITIONS                                 */
/*                                                                                                          */
/************************************************************************************************************/

void PITChannel0Init(void)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    uint32_t pitSourceClock = CLOCK_GetFreq(kCLOCK_PerClk);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(10U, pitSourceClock));

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    PIT_StartTimer(PIT, kPIT_Chnl_0);
}



void PITChannel0ISR(void)
{
    static uint_fast8_t msTicks = 0;

    if(msTicks++ >= 100)    // Interrupt after every 1ms
    {
        LocalIO_DI_CheckPinsState();
        LocalIO_DI_CountersHandler();
    }
}