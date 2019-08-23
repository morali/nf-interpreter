
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ISMA_LOCALIO_H_
#define _ISMA_LOCALIO_H_

#include "spi.h"
#include "fsl_gpio.h"
#include "fsl_lpspi_freertos.h"

typedef enum {
    UI1,
    UI2,
    UI3,
    UI4,
    UI5,
    UI6,
    UI7,
    UI8,
    VREF
} UIChannel_t;

#define DIGITAL_OUTPUT_PORTS 5

extern spi_t s_spi3;

void vLocalIOThread(void * argument);

uint32_t GetDONumber();

bool GetDO(uint32_t);
void SetDO(bool, uint32_t);
bool ToggleDO(uint32_t);

/**
 * @brief  Switches the analog multiplexer to the appropriate channel
 * @note   
 * @param  channel: UIChannel_t
 * @retval None
 */
void SetUIChannel(UIChannel_t channel);

/**
 * @brief  Enable or disable pullup for resistance measurement
 * @note   
 * @param  channel: channel number
 * @param  enable: if true then pullup is enabled
 * @retval None
 */
void SetUIChannelPullup(UIChannel_t channel, bool enable);

typedef struct _local_io
{
    uint8_t ui_input;
    uint8_t analog_output;
    uint8_t digital_output;
} local_io_t;

#endif  //_ISMA_LOCALIO_H_

#ifdef __cplusplus
}
#endif