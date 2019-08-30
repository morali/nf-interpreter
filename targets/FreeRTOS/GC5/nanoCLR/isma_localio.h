
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
    TaskHandle_t Task1us;
    TaskHandle_t Task10us;
    TaskHandle_t Task10ms;
} local_io_tasks_t;

uint32_t GetDONumber();

void vLocalIOThread(void * argument);
void vTimeredTask1us(void * argument);
void vTimeredTask10ms(void * argument);
void vTimeredTask10us(void * argument);

#endif  //_ISMA_LOCALIO_H_

#ifdef __cplusplus
}
#endif
