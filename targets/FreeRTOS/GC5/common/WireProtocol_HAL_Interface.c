//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "board.h"

#include <nanoHAL_v2.h>
#include <WireProtocol.h>
#include <WireProtocol_Message.h>
#include <WireProtocol_HAL_interface.h>

/* Debugging through USB port */
#ifdef HAL_USE_SERIAL_USB
#include "usb.h"
#include "usb_vcom.h"
#include "stream_buffer.h"
#include "usb_device_config.h"

/* Debugging through serial port */
#elif defined HAL_USE_SERIAL
#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"
#endif

WP_Message inboundMessage;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// The functions bellow are the ones that need to be ported to new channels/HALs when required
// These are to be considered as a reference implementations when working on new ports
// 
// This reference implementation provides communication through:
// - serial port (UART/USART) 
// - USB CDC serial port
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HAL_USE_SERIAL_USB

extern usb_data_t* s_cdc_data_p;
extern usb_cdc_vcom_struct_t* s_cdcVcom_p;

int WP_ReceiveBytes(uint8_t* ptr, uint16_t* size)
{    
    size_t bytes_received = 0;
    uint16_t requested_bytes = *size;

    if (requested_bytes == 0)
    {
        /* TODO: Check if this safecheck is needed. */
        /* Wire Protocol may starve other threads while receiving zero bytes packets. */

        // vTaskDelay(pdMS_TO_TICKS(1)); /* Decide how responsive USB is, vs how much processing time it consumes */
        return true;
    }
    
    xStreamBufferSetTriggerLevel(s_cdc_data_p->data_in, requested_bytes);
    bytes_received = xStreamBufferReceive(s_cdc_data_p->data_in, (void *) ptr, requested_bytes, portMAX_DELAY);

    ptr  += bytes_received;
    *size -= bytes_received;
    return (bytes_received > 0); /* Watch performance and how this affects transmission */
}

int WP_TransmitMessage(WP_Message* message)
{
    usb_status_t error = kStatus_USB_Error;

    s_cdc_data_p->xWriteToNotify = xTaskGetCurrentTaskHandle();

    uint8_t * send_addr = (uint8_t *) &(message->m_header);
    uint32_t send_size = sizeof(message->m_header);

    /* Check for buffer overflow */
    if ((send_size + message->m_header.m_size) > sizeof(s_cdc_data_p->s_currSendBuf) / sizeof(uint8_t))
    { 
        configASSERT(1);
    }

    memcpy(s_cdc_data_p->s_currSendBuf, send_addr, send_size);
    memcpy(s_cdc_data_p->s_currSendBuf + send_size, (uint8_t *) (message->m_payload), message->m_header.m_size);

    send_size = send_size + message->m_header.m_size;

    /* for now we ignore if USB driver is busy and return false (code below conflicts with resetting the board through VS)
    while (error != kStatus_USB_Success)
    {
        error = USB_DeviceSendRequest(g_composite_p->deviceHandle, vcomInstance->bulkInEndpoint, s_cdc_data_p->s_currSendBuf[0], send_size);
        vTaskDelay(pdMS_TO_TICKS(1));
        taskYIELD();
    }
    */ 

    error = USB_DeviceSendRequest(s_cdcVcom_p->deviceHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, s_cdc_data_p->s_currSendBuf, send_size);
    if (error != kStatus_USB_Success) 
        return false;
    

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    return true; 
}

#elif defined HAL_USE_SERIAL

bool WP_Port_Intitialised = false;

static lpuart_rtos_handle_t handle;
static struct _lpuart_handle t_handle;
uint8_t background_buffer[1024];

lpuart_rtos_config_t lpuart_config = {
    .baudrate = 921600,
    .parity = kLPUART_ParityDisabled,
    .stopbits = kLPUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

bool WP_Initialise()
{
 
    NVIC_SetPriority(BOARD_UART_IRQ, 5);

    lpuart_config.srcclk = BOARD_DebugConsoleSrcFreq();
    lpuart_config.base = LPUART8;

    int ret = LPUART_RTOS_Init(&handle, &t_handle, &lpuart_config);
    WP_Port_Intitialised = (ret == 0);
 
    if (!WP_Port_Intitialised) {
        vTaskSuspend(NULL);
    }
    
    return WP_Port_Intitialised;
}


int WP_ReceiveBytes(uint8_t* ptr, uint16_t* size)
{
    // TODO: Initialise Port if not already done, Wire Protocol should be calling this directly at startup
    if (!WP_Port_Intitialised) WP_Initialise();

    // save for latter comparison
    uint16_t requestedSize = *size;
    
    //int readData = 0;
    // sanity check for request of 0 size
    if(*size)
    {
        size_t read = 0;
        LPUART_RTOS_Receive(&handle, ptr, *size , &read);
        
        ptr  += read;
        *size -= read;

        // check if the requested read matches the actual read count
        return (requestedSize == read);
    }

    return true;
}

int WP_TransmitMessage(WP_Message* message)
{

    if (!WP_Port_Intitialised) WP_Initialise();

    if (kStatus_Success !=
                LPUART_RTOS_Send(&handle, (uint8_t *)&message->m_header, sizeof(message->m_header))) {
                    return false;
                }
    

    // if there is anything on the payload send it to the output stream
    if(message->m_header.m_size && message->m_payload)
    {
        if (kStatus_Success !=
                LPUART_RTOS_Send(&handle, (uint8_t *)message->m_payload, message->m_header.m_size)) {
                    return false;
                }
    }

    return true;    
}

#else
#error "Wire Protocol needs a transport. Please make sure that HAL_USE_SERIAL and/or HAL_USE_SERIAL_USB are defined."
#endif