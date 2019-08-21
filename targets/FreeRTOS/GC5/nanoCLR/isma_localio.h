
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ISMA_LOCALIO_H_
#define _ISMA_LOCALIO_H_

#include "fsl_gpio.h"
#include "fsl_lpspi_freertos.h"

/* Clock divider for master lpspi clock source */
#define LPSPI3_CLOCK_SOURCE_DIVIDER (7U)
#define LPSPI3_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / (LPSPI3_CLOCK_SOURCE_DIVIDER + 1U))

#define LPSPI3_IRQ_PRIO 6U
#define LPSPI3_BAUDRATE 2000000U
#define LPSPI3_BUFFSIZE 3U

#define DO_OutputsNo (sizeof(DO_Ouputs) / sizeof(DO_Ouputs[0]))
// #define AO_utputsNo (sizeof(AOOutputs) / sizeof(AOOutputs[0]))
// #define DI_InputsNo (sizeof(DInputs) / sizeof(DInputs[0]))
// #define UI_InputsNo (sizeof(UInputs) / sizeof(UInputs[0]))

void SPI3Init();

uint32_t GetDONumber();
const char *GetDOName(uint32_t DONumber);

bool GetDO(uint32_t);

void SetDO(bool, uint32_t);

typedef struct _local_io
{
    uint8_t ui_input;
    uint8_t analog_output;
    uint8_t digital_ouput;
} local_io_t;

#endif  //_ISMA_LOCALIO_H_

#ifdef __cplusplus
}
#endif