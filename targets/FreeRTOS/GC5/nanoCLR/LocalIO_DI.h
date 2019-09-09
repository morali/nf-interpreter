/*
 * localIO_DI.h
 *
 *  Created on: 22.08.2019
 *      Author: Jakub Standarski
 */

#ifndef LOCALIO_DI_
#define LOCALIO_DI_

#include <stdbool.h>
#include <stdint.h>

void InitDI(void);

#ifdef __cplusplus
extern "C" {
#endif
void DIReadPinsState(void);
void DICountersHandler(void);
uint32_t GetDINumber();
bool GetDIState(uint32_t id);
uint32_t GetDICounter(uint32_t id);
void SetDICounter(uint32_t id, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* LOCALIO_DI_ */
