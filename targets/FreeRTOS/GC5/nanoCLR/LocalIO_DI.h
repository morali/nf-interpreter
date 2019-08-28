/*
 * localIO_DI.h
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#ifndef LOCALIO_DI_
#define LOCALIO_DI_

#ifdef __cplusplus
extern "C" {
#endif

#include "fsl_gpio.h"



/************************************************************************************************************/
/*                                                                                                          */
/*                                           PUBLIC DEFINES                                                 */
/*                                                                                                          */
/************************************************************************************************************/

#define DIGITAL_INPUTS_AMOUNT   4




/************************************************************************************************************/
/*                                                                                                          */
/*                                          PUBLIC VARIABLES                                                */
/*                                                                                                          */
/************************************************************************************************************/






/************************************************************************************************************/
/*                                                                                                          */
/*                                LOCALIO_DI PUBLIC FUNCTIONS DECLARATIONS                                  */
/*                                                                                                          */
/************************************************************************************************************/

void LocalIO_DI_Init(void);

void LocalIO_DI_CheckPinsState(void);

void LocalIO_DI_CountersHandler(void);

bool LocalIO_DI_ReadInput(uint8_t diPinNumber);



#ifdef __cplusplus
}
#endif

#endif /* LOCALIO_DI_ */
