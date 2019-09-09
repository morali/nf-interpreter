
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ISMA_LOCALIO_DO_H_
#define _ISMA_LOCALIO_DO_H_

#include "isma_localio.h"

#define DIGITAL_OUTPUT_PORTS 5

uint32_t GetDONumber();
bool GetDO(uint32_t);
void SetDO(bool, uint32_t);
void ToggleDO(uint32_t);

#endif  //_ISMA_LOCALIO_DO_H_

#ifdef __cplusplus
}
#endif