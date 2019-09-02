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
extern localIO_AO_t s_local_ao;
extern local_io_t s_local_io_tx;
extern spi_t s_spi3;

void PITChannel0Init(void)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    // uint32_t pitSourceClock = CLOCK_GetFreq(kCLOCK_PerClk);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, SPI3_TIMER_PERIOD);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, PWM_TIMER_PERIOD);
    // PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, ADC_TIMER_PERIOD);

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    // PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
    
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, PIT_IRQ_PRIO);

    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    // PIT_StartTimer(PIT, kPIT_Chnl_2);    
}

void PIT_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Interrupt every 1us */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_0))
    {
		/* Clear interrupt flag.*/       
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

        /* SPI ring transfer, reads digital inputs and sets digital and analog outputs (PWMs) */
        LPSPI_MasterTransferNonBlocking(LPSPI3, &s_spi3.masterHandle, &s_spi3.spi_transfer);
		LPSPI_RTOS_Transfer(&s_spi3.masterRtosHandle, &s_spi3.spi_transfer);
        GPIO_WritePinOutput(GPIO1, 28, 1);
        GPIO_WritePinOutput(GPIO1, 28, 0);
	}

    /* Interrupt every 10us */
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
		
        if ((s_local_ao.pwm_count == s_local_ao.config[0].duty_cycle) && s_local_ao.config[0].PWM)
        {
            s_local_io_tx.analog_output ^= 0x2;
        }
		if ((s_local_ao.pwm_count == s_local_ao.config[1].duty_cycle) && s_local_ao.config[1].PWM)
		{
			s_local_io_tx.analog_output ^= 0x4;
		}
		if ((s_local_ao.pwm_count == s_local_ao.config[2].duty_cycle) && s_local_ao.config[2].PWM)
		{
			s_local_io_tx.analog_output ^= 0x8;
		}
		
		if (s_local_ao.pwm_count >= 100)
        {
			s_local_ao.pwm_count = 0;
            s_local_io_tx.analog_output ^= 0x2;
            s_local_io_tx.analog_output ^= 0x4;
            s_local_io_tx.analog_output ^= 0x8;
        }
        
		s_local_ao.pwm_count++;
	}

     /* Interrupt every 1s (quick check of digital output ports) */
	if(PIT_GetStatusFlags(PIT, kPIT_Chnl_2))
    {
		/* Clear interrupt flag.*/		
        vTaskNotifyGiveFromISR(local_io_tasks.Task1s, &xHigherPriorityTaskWoken);
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

#ifdef __cplusplus
}
#endif