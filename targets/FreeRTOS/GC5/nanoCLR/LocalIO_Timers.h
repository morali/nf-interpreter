/*
 * localIO_Timers.h
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "fsl_pit.h"
#include "spi.h"
#include "isma_localio.h"
#include "LocalIO_AO.h"

#define _100US_TIMER_PERIOD USEC_TO_COUNT(100, CLOCK_GetFreq(kCLOCK_PerClk))
#define _1MS_TIMER_PERIOD USEC_TO_COUNT(1000, CLOCK_GetFreq(kCLOCK_PerClk))

#define PIT_IRQ_PRIO 6

void PITChannel0Init(void);
void PIT_IRQHandler(void);
