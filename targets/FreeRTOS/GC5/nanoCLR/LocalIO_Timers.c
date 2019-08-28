/*
 * localIO_Timers.c
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "LocalIO_Timers.h"

extern local_io_tasks_t local_io_tasks;


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
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, SPI3_TIMER_PERIOD);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, ADC_POLLING_TIMER_PERIOD);

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
    
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_2);    

     /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);    
    NVIC_SetPriority(PIT_IRQn, 8);
}

void PIT_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Przerwanie co 10us */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_0))
    {
		/* Clear interrupt flag.*/		
        vTaskNotifyGiveFromISR(local_io_tasks.Task10us, &xHigherPriorityTaskWoken);
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

    /* Przerwanie co 10ms */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_2))
    {
		/* Clear interrupt flag.*/		
        vTaskNotifyGiveFromISR(local_io_tasks.Task10ms, &xHigherPriorityTaskWoken);
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}