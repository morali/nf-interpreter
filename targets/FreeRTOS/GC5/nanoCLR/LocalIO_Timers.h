/*
 * localIO_Timers.h
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "fsl_pit.h"
#include "spi.h"
#include "isma_localio.h"
#include "LocalIO_AO_TO.h"

#define _50US_TIMER_PERIOD 2500
#define _100US_TIMER_PERIOD 5000
#define _1MS_TIMER_PERIOD 50000 

#define PIT_IRQ_PRIO 6

void PITChannel0Init(void);
void PIT_IRQHandler(void);
