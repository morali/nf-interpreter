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
#include "LocalIO_DI.h"

extern localIO_AO_t s_local_ao;
extern local_io_t s_local_io_tx;
extern spi_t s_spi3;

void PITChannel0Init(void)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    // uint32_t pitSourceClock = CLOCK_GetFreq(kCLOCK_PerClk);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, _50US_TIMER_PERIOD);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, _100US_TIMER_PERIOD);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, _1MS_TIMER_PERIOD);

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, PIT_IRQ_PRIO);

    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
}

void PIT_IRQHandler(void)
{
    /* Interrupt every 1us */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_0))
    {
		/* Clear interrupt flag.*/
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        // if(!LPSPI_GetStatusFlags(LPSPI3))
        /* SPI ring transfer, reads digital inputs and sets digital and analog outputs (PWMs) */
        LPSPI_MasterTransferNonBlocking(LPSPI3, &s_spi3.masterHandle, &s_spi3.spi_transfer);        
	}

    /* Interrupt every 100us */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_1))
    {
		/* Clear interrupt flag.*/
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);

        /* PWM Controller */

        /*
        1. Check timer
        2. Increment timer
        3. If timer > 100, reset timer.
        */


        for (uint32_t i = 0; i < ANALOG_OUTPUT_PORTS; i++)
        {
            if((s_local_ao.pwm_count >= s_local_ao.AOconfig[i].duty_cycle)  && s_local_ao.AOconfig[i].mode == PWM)
            {
                s_local_io_tx.analog_output = 1U << (i + 1);
            }
            else
            {
                s_local_io_tx.analog_output = 0U << (i + 1);
            }
        }

		if (s_local_ao.pwm_count >= 1000000)
        {
			s_local_ao.pwm_count = 0;
        }
		s_local_ao.pwm_count++;
	}

    /* Interrupt every 1ms */
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_2)) {
        /* Clear interrupt flag.*/
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);

        DIReadPinsState();
        DICountersHandler();
    }
}

#ifdef __cplusplus
}
#endif
