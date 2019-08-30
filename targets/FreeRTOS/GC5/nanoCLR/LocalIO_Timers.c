/*
 * localIO_Timers.c
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "LocalIO_Timers.h"

extern local_io_tasks_t local_io_tasks;

void PITChannel0Init(void)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    // uint32_t pitSourceClock = CLOCK_GetFreq(kCLOCK_PerClk);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, SPI3_TIMER_PERIOD);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, PWM_TIMER_PERIOD);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, ADC_TIMER_PERIOD);

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
    
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, 10);

    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_2);    
}

void PIT_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Przerwanie co 10us */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_0))
    {
		/* Clear interrupt flag.*/		
        vTaskNotifyGiveFromISR(local_io_tasks.Task1us, &xHigherPriorityTaskWoken);
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

    /* Przerwanie co 10ms */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_1))
    {
		/* Clear interrupt flag.*/		
        vTaskNotifyGiveFromISR(local_io_tasks.Task10us, &xHigherPriorityTaskWoken);
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
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

#ifdef __cplusplus
}
#endif