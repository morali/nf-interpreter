//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#ifndef _WIN_DEV_SERIAL_NATIVE_TARGET_H_
#define _WIN_DEV_SERIAL_NATIVE_TARGET_H_

#define LPUART_TX_DMA_CHANNEL 0U
#define UART_DMA_INTERRUPT_PRIO 8U
#define UART_INTERRUPT_PRIO 7U

#include <target_windows_devices_serialcommunication_config.h>
#include <win_dev_serial_native.h>

#include "fsl_lpuart.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

typedef struct
{
    uint8_t dma_num;

    lpuart_config_t uartCfg;
    edma_config_t edmaCfg;

    lpuart_edma_handle_t g_lpuartEdmaHandle;
    edma_handle_t g_lpuartTxEdmaHandle;
    lpuart_handle_t g_lpuartRxHandle;

    HAL_RingBuffer<uint8_t> TxRingBuffer;
    uint8_t* TxBuffer;
    uint16_t TxOngoingCount;
    
    HAL_RingBuffer<uint8_t> RxRingBuffer;
    uint8_t* RxBuffer;
    uint16_t RxBytesToRead;
    uint8_t WatchChar;

    TaskHandle_t xRWTaskToNotify;

} NF_PAL_UART;

#endif  //_WIN_DEV_SERIAL_NATIVE_TARGET_H_
