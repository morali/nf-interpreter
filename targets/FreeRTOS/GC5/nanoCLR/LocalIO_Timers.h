/*
 * localIO_Timers.h
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#ifndef _ISMA_LOCALIO_TIMERS_H_
#define _ISMA_LOCALIO_TIMERS_H_

#define _100US_TIMER_PERIOD USEC_TO_COUNT(100, CLOCK_GetFreq(kCLOCK_PerClk))
#define _1MS_TIMER_PERIOD USEC_TO_COUNT(1000, CLOCK_GetFreq(kCLOCK_PerClk))

#define PIT_IRQ_PRIO 6

void PITChannel0Init(void);

#endif //_ISMA_LOCALIO_TIMERS_H_