
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ISMA_LOCALIO_H_
#define _ISMA_LOCALIO_H_

#include "spi.h"
#include "fsl_gpio.h"
#include "fsl_lpspi_freertos.h"

typedef struct _local_io
{
    uint8_t ui_input;
    uint8_t analog_output;
    uint8_t digital_output;
} local_io_t;

typedef struct _local_io_tasks
{
    TaskHandle_t Task1s;
} local_io_tasks_t;

uint32_t GetDONumber();

void vLocalIOThread(void * argument);
void vTimeredTask1s(void * argument);

#endif  //_ISMA_LOCALIO_H_

#ifdef __cplusplus
}
#endif
