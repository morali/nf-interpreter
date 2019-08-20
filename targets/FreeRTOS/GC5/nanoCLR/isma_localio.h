
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

static const char *DOuputs[] = {
  [0] = "DO1", 
  [1] = "DO2", 
  [2] = "DO3", 
  [3] = "DO4",
  [4] = "DO5"
};
#define DOutputsNo (sizeof(DOuputs) / sizeof(DOuputs[0]))

static const char *AOutputs[] = {
  [0] = "UI1",
  [1] = "UI2",
  [2] = "UI3",
  [3] = "UI4"
};
#define AOutputsNo (sizeof(AOutputs) / sizeof(AOutputs[0]))

static const char *DInputs[] = {
  [0] = "DI1",
  [1] = "DI2",
  [2] = "DI3",
  [3] = "DI4"
};
#define DInputsNo (sizeof(DInputs) / sizeof(DInputs[0]))

static const char *UInputs[] = {
  [0] = "UI1",
  [1] = "UI2",
  [2] = "UI3",
  [3] = "UI4"
};
#define UInputsNo (sizeof(UInputs) / sizeof(UInputs[0]))

void SPI3Init();

uint32_t getDOutputNumber();

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