/*
 * localIO_Timers.c
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "LocalIO_Timers.h"
#include "LocalIO.h"
#include "LocalIO_AO_TO.h"
#include "LocalIO_DI.h"
#include "fsl_pit.h"

static localIO_AO_TO_t *local_ao_to;
static local_io_t *local_io_tx;

void PITChannel0Init(void) {
  pit_config_t pitConfig;
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);

  // uint32_t pitSourceClock = CLOCK_GetFreq(kCLOCK_PerClk);
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, _100US_TIMER_PERIOD);
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, _1MS_TIMER_PERIOD);

  PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
  PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);

  /* Enable at the NVIC */
  EnableIRQ(PIT_IRQn);
  NVIC_SetPriority(PIT_IRQn, PIT_IRQ_PRIO);

  PIT_StartTimer(PIT, kPIT_Chnl_0);
  PIT_StartTimer(PIT, kPIT_Chnl_2);

  local_ao_to = GetTOAOConfig();
  local_io_tx = GetLocalIoTx();
}

static const uint16_t pwmMultipliers[] = {100, 10, 1, 1000, 10000};
static const uint8_t pwmAOPin[ANALOG_OUTPUT_PORTS] = {1 << 3, 1 << 2, 1 << 1};
static const uint8_t pwmTOPin[TRIAC_OUTPUT_PORTS] = {1 << 4, 1 << 5};

void PIT_IRQHandler(void) {
  /* Interrupt every 100us */
  if (PIT_GetStatusFlags(PIT, kPIT_Chnl_0)) {
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

    for (uint32_t i = 0; i < ANALOG_OUTPUT_PORTS; i++) {
      if (local_ao_to->AOconfig[i].mode == AO_PWM) {
        uint32_t value = local_ao_to->AOconfig[i].duty_cycle * pwmMultipliers[local_ao_to->AOconfig[i].frequency];

        if ((local_ao_to->AOconfig[i].pwm_count >= value)) {
          local_io_tx->analog_output &= ~pwmAOPin[i];
        } else {
          local_io_tx->analog_output |= pwmAOPin[i];
        }

        if (++local_ao_to->AOconfig[i].pwm_count >= pwmMultipliers[local_ao_to->AOconfig[i].frequency] * 100) {
          local_ao_to->AOconfig[i].pwm_count = 0;
        }
      } else {
        local_io_tx->analog_output &= ~pwmAOPin[i];
      }
    }

    for (uint32_t i = 0; i < TRIAC_OUTPUT_PORTS; i++) {
      if (local_ao_to->TOconfig[i].mode == TO_PWM) {
        uint32_t value = local_ao_to->TOconfig[i].duty_cycle * pwmMultipliers[local_ao_to->TOconfig[i].frequency];

        if ((local_ao_to->TOconfig[i].pwm_count >= value)) {
          local_io_tx->analog_output &= ~pwmTOPin[i];
        } else {
          local_io_tx->analog_output |= pwmTOPin[i];
        }

        if (++local_ao_to->TOconfig[i].pwm_count >= pwmMultipliers[local_ao_to->TOconfig[i].frequency] * 100) {
          local_ao_to->TOconfig[i].pwm_count = 0;
        }
      } else if (local_ao_to->TOconfig[i].mode == TO_Digital) {

        if (local_ao_to->TOconfig[i].digital) {
          local_io_tx->analog_output |= pwmTOPin[i];
        } else {
          local_io_tx->analog_output &= ~pwmTOPin[i];
        }
      }
    }
  }

  /* Interrupt every 1ms */
  if (PIT_GetStatusFlags(PIT, kPIT_Chnl_2)) {
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);

    DIReadPinsState();
    DICountersHandler();
  }
}
