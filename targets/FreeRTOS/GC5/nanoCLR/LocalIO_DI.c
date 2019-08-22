/*
 * localIO_DI.c
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#include "localIO_DI.h"



/************************************************************************************************************/
/*                                                                                                          */
/*                                           PRIVATE DEFINES                                                */
/*                                                                                                          */
/************************************************************************************************************/

#define DIGITAL_INPUTS_AMOUNT   4





/************************************************************************************************************/
/*                                                                                                          */
/*                                          PRIVATE VARIABLES                                               */
/*                                                                                                          */
/************************************************************************************************************/

static uint8_t  digitalInputState;
static uint32_t digitalInputCounter[DIGITAL_INPUTS_AMOUNT];






/************************************************************************************************************/
/*                                                                                                          */
/*                                LOCALIO_DI PUBLIC FUNCTIONS DEFINITIONS                                   */
/*                                                                                                          */
/************************************************************************************************************/

void LocalIO_DI_Init(void)
{
    const gpio_pin_config_t gpioConfig = 
    {
        .direction      = kGPIO_DigitalInput,
        .outputLogic    = 0,
        .interruptMode  = kGPIO_NoIntmode
    };

    GPIO_PinInit(GPIO2, 28, &gpioConfig);
    GPIO_PinInit(GPIO2, 29, &gpioConfig);
    GPIO_PinInit(GPIO2, 30, &gpioConfig);
    GPIO_PinInit(GPIO2, 31, &gpioConfig);
}



void LocalIO_DI_CheckPinsState(void)
{
    for(uint8_t pinNumber = 0; pinNumber < DIGITAL_INPUTS_AMOUNT; pinNumber++)
    {
        if(GPIO_PinRead(GPIO2, pinNumber + 28))
        {
            digitalInputState |= (1 << pinNumber);
        }
        else
        {
            digitalInputState &= ~(1 << pinNumber);
        }
    }
}



void LocalIO_DI_CountersHandler(void)
{
    static uint16_t inputLastState = 0;
    uint16_t inputChange = 0;

    inputChange    = digitalInputState ^ inputLastState;
    inputLastState = digitalInputState;

    for(uint8_t i = 0; i < DIGITAL_INPUTS_AMOUNT; i++)
    {
        // TODO: check counterResetFlag
        if((inputChange & (1 << i)) && (digitalInputState & (1 << i)))
        {
            digitalInputCounter[i]++;
        }
    }
}



uint8_t LocalIO_DI_ReadInput(uint8_t pinNumber)
{
    return GPIO_PinRead(GPIO2, pinNumber);
}


