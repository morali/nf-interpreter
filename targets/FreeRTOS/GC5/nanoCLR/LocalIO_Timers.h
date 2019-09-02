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

#define SPI3_TIMER_PERIOD 10
#define PWM_TIMER_PERIOD 1000
#define ADC_TIMER_PERIOD 100000000 /* FOR NOW 0,5SEC PERIOD */

#define PIT_IRQ_PRIO 5

void PITChannel0Init(void);
void PIT_IRQHandler(void);
