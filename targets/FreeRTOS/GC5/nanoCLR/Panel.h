/*
 * Panel.h
 *
 *  Created on: 09.08.2019
 *      Author: Jakub Standarski
 */

#ifndef PANEL_H_
#define PANEL_H_

#include <stdbool.h>
#include <stdint.h>

/************************************************************************************************************/
/*                                                                                                          */
/*                                 PANEL PUBLIC FUNCTIONS DECLARATIONS                                      */
/*                                                                                                          */
/************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

uint32_t getLedNumber();
const char *GetLedName(uint32_t ledNumber);
void SetLed(uint32_t ledNumber, bool state);
bool GetLed(uint32_t ledNumber);

uint32_t getDipswitchNumber();
uint8_t GetDipswitch(uint32_t dipswitchNumber);
const char *GetDipswitchName(uint32_t dipswitchNumber);

#ifdef __cplusplus
}
#endif

/* FreeRTOS task declaration */
void vPanelThread(void *pvParameters);

#endif /* PANEL_H_ */