/*
 * localIO_Timers.h
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "fsl_pit.h"
#include "isma_localio.h"



/************************************************************************************************************/
/*                                                                                                          */
/*                                           PUBLIC DEFINES                                                 */
/*                                                                                                          */
/************************************************************************************************************/

#define SPI3_TIMER_PERIOD USEC_TO_COUNT(1000U, pitSourceClock)
#define ADC_POLLING_TIMER_PERIOD USEC_TO_COUNT(1000000U, pitSourceClock) /* FOR NOW 0,5SEC PERIOD */
// #define ADC_POLLING_TIMER_PERIOD 0x2FAF07F /* FOR NOW 0,5SEC PERIOD */





/************************************************************************************************************/
/*                                                                                                          */
/*                                          PUBLIC VARIABLES                                                */
/*                                                                                                          */
/************************************************************************************************************/








/************************************************************************************************************/
/*                                                                                                          */
/*                              LOCALIO_TIMERS PUBLIC FUNCTIONS DECLARATIONS                                */
/*                                                                                                          */
/************************************************************************************************************/

void PITChannel0Init(void);
void PIT_IRQHandler(void);
