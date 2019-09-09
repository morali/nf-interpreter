/*
 * Copyright 2017, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "usb_vcom.h"

/*******************************************************************************
* Variables
******************************************************************************/

usb_device_composite_struct_t * g_composite_p;
usb_data_t * s_cdc_data_p;

/* Data structure of wire protocol usb device */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) usb_data_t s_cdc_data;

/* Composite device structure. */
usb_device_composite_struct_t g_composite;

/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_lineCoding[USB_DEVICE_CONFIG_CDC_ACM][LINE_CODING_SIZE] = {
    {/* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
     (LINE_CODING_DTERATE >> 0U) & 0x000000FFU, (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
     (LINE_CODING_DTERATE >> 16U) & 0x000000FFU, (LINE_CODING_DTERATE >> 24U) & 0x000000FFU, LINE_CODING_CHARFORMAT,
     LINE_CODING_PARITYTYPE, LINE_CODING_DATABITS},
    {/* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
     (LINE_CODING_DTERATE >> 0U) & 0x000000FFU, (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
     (LINE_CODING_DTERATE >> 16U) & 0x000000FFU, (LINE_CODING_DTERATE >> 24U) & 0x000000FFU, LINE_CODING_CHARFORMAT,
     LINE_CODING_PARITYTYPE, LINE_CODING_DATABITS},
};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_abstractState[USB_DEVICE_CONFIG_CDC_ACM][COMM_FEATURE_DATA_SIZE] = {
    {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU, (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU},
    {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU, (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU},
};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_countryCode[USB_DEVICE_CONFIG_CDC_ACM][COMM_FEATURE_DATA_SIZE] = {
    {(COUNTRY_SETTING >> 0U) & 0x00FFU, (COUNTRY_SETTING >> 8U) & 0x00FFU},
    {(COUNTRY_SETTING >> 0U) & 0x00FFU, (COUNTRY_SETTING >> 8U) & 0x00FFU},
};

/* CDC ACM information */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_cdc_acm_info_t s_usbCdcAcmInfo[USB_DEVICE_CONFIG_CDC_ACM] = {
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, 0}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, 0},
};

/*******************************************************************************
* Code
******************************************************************************/

usb_status_t USB_Init(void)
{
    usb_status_t error = kStatus_USB_Error;

    /* Initialize data buffers for incoming data. */
    s_cdc_data.data_in[0] = xStreamBufferCreate(STREAM_RECV_USB_BUFFER, HS_CDC_VCOM_BULK_IN_PACKET_SIZE);
    
    #ifdef USB_CONSOLE_DEBUG
        s_cdc_data.data_in[1] = xStreamBufferCreate(STREAM_CONSOLE_USB_BUFFER, HS_CDC_VCOM_BULK_IN_PACKET_SIZE);   
    #endif

    USB_DeviceClockInit();

    g_composite_p = &g_composite;
    s_cdc_data_p = &s_cdc_data;

    g_composite_p->speed = USB_SPEED_HIGH;
    g_composite_p->attach = 0;
    g_composite_p->deviceHandle = NULL;

    error = USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback,(usb_device_handle *) &g_composite_p->deviceHandle);
    if (error != kStatus_USB_Success)
        return error;

    error = USB_DeviceCdcVcomInit((usb_device_composite_struct_t *) g_composite_p);
    if (error != kStatus_USB_Success)
        return error;

    USB_DeviceIsrEnable();

    error = USB_DeviceRun(g_composite_p->deviceHandle);
    if (error != kStatus_USB_Success)
        return error;

    #ifdef USB_CONSOLE_DEBUG
        BaseType_t xReturned = xTaskCreate(vcom_debug_thread, "USBConsole", 128, NULL, USB_CONSOLE_THREAD_PRIO, &s_cdc_data.xReadToNotify[1]);
        if (xReturned != pdPASS)
            error = kStatus_USB_Error;
        g_composite_p->cdcVcom[1].startTransactions = 1;
    #endif

    g_composite_p->attach = 1;
    g_composite_p->cdcVcom[0].startTransactions = 1;

    /* If USB init is sucessful unlock receiver thread  */
    vTaskNotifyGiveFromISR(s_cdc_data.xReceiverTask, pdFALSE);

    return kStatus_USB_Success;
}

#ifdef USB_CONSOLE_DEBUG

void vcom_debug_thread(void * argument)
{
    (void) argument;
    /* For future implementation of reading USB debug console. */

    s_cdc_data.xReadToNotify[1] = xTaskGetCurrentTaskHandle();
    
    vTaskDelete(NULL);
    while(1)
    {
        /* STUB */
    }
}

#endif

/*!
 * @brief Bulk out pipe callback function.
 *
 * This function serves as the callback function for bulk out pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmBulkOut(usb_device_handle handle,
                                     usb_device_endpoint_callback_message_struct_t *message,
                                     void *callbackParam)
{
    usb_cdc_vcom_struct_t *vcomInstance;
    usb_status_t error = kStatus_USB_Error;
    uint8_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for (i = 0; i < USB_DEVICE_CONFIG_CDC_ACM; i++)
    {
        if (*((uint8_t *)callbackParam) == g_composite_p->cdcVcom[i].dataInterfaceNumber)
        {
            break;
        }
    }
    if (i >= USB_DEVICE_CONFIG_CDC_ACM)
    {
        return error;
    }

    #ifdef USB_CONSOLE_DEBUG
        return error;
    #endif

    if (i == 1)
        return kStatus_USB_Success;

    vcomInstance = &g_composite_p->cdcVcom[i];

    /* Check if message is there and USB transaction are started */
    if (vcomInstance->startTransactions == 1 && message != NULL && message->buffer != NULL && message->length <= 512)
    {
        xStreamBufferSendFromISR(s_cdc_data.data_in[0], message->buffer, message->length, &xHigherPriorityTaskWoken);
    }
    USB_DeviceRecvRequest(handle, vcomInstance->bulkOutEndpoint, vcomInstance->currRecvBuf, vcomInstance->bulkOutEndpointMaxPacketSize);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
    return kStatus_USB_Success;
}

/*!
 * @brief Bulk in pipe callback function.
 *
 * This function serves as the callback function for bulk in pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmBulkIn(usb_device_handle handle,
                                    usb_device_endpoint_callback_message_struct_t *message,
                                    void *callbackParam)
{
    usb_cdc_vcom_struct_t *vcomInstance;
    usb_status_t error = kStatus_USB_Error;
    uint8_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for (i = 0; i < USB_DEVICE_CONFIG_CDC_ACM; i++)
    {
        if (*((uint8_t *)callbackParam) == g_composite_p->cdcVcom[i].dataInterfaceNumber)
        {
            break;
        }
    }
    if (i >= USB_DEVICE_CONFIG_CDC_ACM)
    {
        return error;
    }
    vcomInstance = &g_composite_p->cdcVcom[i];
    
    if ((message->length != 0) && (!(message->length % vcomInstance->bulkInEndpointMaxPacketSize)))
    {
        /* If the last packet is the size of endpoint, then send also zero-ended packet,
         ** meaning that we want to inform the host that we do not have any additional
         ** data, so it can flush the output.
         */
        USB_DeviceSendRequest(handle, vcomInstance->bulkInEndpoint, NULL, 0);
    }
    else if (1 == vcomInstance->startTransactions)
    {
        if ((message->buffer != NULL) || ((message->buffer == NULL) && (message->length == 0)))
        {
            vTaskNotifyGiveFromISR( s_cdc_data.xWriteToNotify[i], &xHigherPriorityTaskWoken );
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    (void) handle;
    (void) message;
    (void) callbackParam;
    return error;
}

/*!
 * @brief Interrupt in pipe callback function.
 *
 * This function serves as the callback function for interrupt in pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmInterruptIn(usb_device_handle handle,
                                         usb_device_endpoint_callback_message_struct_t *message,
                                         void *callbackParam)
{
    usb_status_t error = kStatus_USB_Error;
    uint8_t i;

    for (i = 0; i < USB_DEVICE_CONFIG_CDC_ACM; i++)
    {
        if (*((uint8_t *)callbackParam) == g_composite_p->cdcVcom[i].communicationInterfaceNumber)
        {
            break;
        }
    }
    if (i >= USB_DEVICE_CONFIG_CDC_ACM)
    {
        return error;
    }

    g_composite_p->cdcVcom[i].hasSentState = 0;
    return kStatus_USB_Success;
    (void) handle;
    (void) message;
}

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    usb_status_t error = kStatus_USB_Error;
    if (status)
    {
        if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            error = USB_DeviceStallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
        {
            error = USB_DeviceStallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            error = USB_DeviceStallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
        {
            error = USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            error = USB_DeviceUnstallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
        {
            error = USB_DeviceUnstallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            error = USB_DeviceUnstallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
        {
            error = USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return error;
}

/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    usb_cdc_vcom_struct_t *vcomInstance;
    usb_cdc_acm_info_t *acmInfo;
    uint32_t len;
    uint8_t *uartBitmap;
    uint8_t i;

    for (i = 0; i < USB_DEVICE_CONFIG_CDC_ACM; i++)
    {
        if (setup->wIndex == g_composite_p->cdcVcom[i].communicationInterfaceNumber)
        {
            break;
        }
    }
    if (i >= USB_DEVICE_CONFIG_CDC_ACM)
    {
        return error;
    }
    vcomInstance = &g_composite_p->cdcVcom[i];
    acmInfo = vcomInstance->usbCdcAcmInfo;
    switch (setup->bRequest)
    {
        case USB_DEVICE_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:
            break;
        case USB_DEVICE_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE:
            break;
        case USB_DEVICE_CDC_REQUEST_SET_COMM_FEATURE:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == setup->wValue)
            {
                *buffer = vcomInstance->abstractState;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == setup->wValue)
            {
                *buffer = vcomInstance->countryCode;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_GET_COMM_FEATURE:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == setup->wValue)
            {
                *buffer = vcomInstance->abstractState;
                *length = COMM_FEATURE_DATA_SIZE;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == setup->wValue)
            {
                *buffer = vcomInstance->countryCode;
                *length = COMM_FEATURE_DATA_SIZE;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_CLEAR_COMM_FEATURE:
            break;
        case USB_DEVICE_CDC_REQUEST_GET_LINE_CODING:
            *buffer = vcomInstance->lineCoding;
            *length = LINE_CODING_SIZE;
            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_SET_LINE_CODING:
            *buffer = vcomInstance->lineCoding;
            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_SET_CONTROL_LINE_STATE:
        {
            acmInfo->dteStatus = setup->wValue;
            /* activate/deactivate Tx carrier */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }

            /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }
            /* Com port of terminal tool running on PC is closed now */
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }

            /* Indicates to DCE if DTE is present or not */
            acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

            /* Initialize the serial state buffer */
            acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                        /* bmRequestType */
            acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_REQUEST_SERIAL_STATE_NOTIF; /* bNotification */
            acmInfo->serialStateBuf[2] = 0x00;                                      /* wValue */
            acmInfo->serialStateBuf[3] = 0x00;
            acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
            acmInfo->serialStateBuf[5] = 0x00;
            acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
            acmInfo->serialStateBuf[7] = 0x00;
            /* Notify to host the line state */
            acmInfo->serialStateBuf[4] = setup->wIndex;
            /* Lower byte of UART BITMAP */
            uartBitmap = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
            uartBitmap[0] = acmInfo->uartState & 0xFFu;
            uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
            len = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
            if (0 == vcomInstance->hasSentState)
            {
                error = USB_DeviceSendRequest(handle, vcomInstance->interruptEndpoint, acmInfo->serialStateBuf, len);
                vcomInstance->hasSentState = 1;
            }
            else
            {
            }
            // /* Update status */
            // if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            // {
            //     /*    To do: CARRIER_ACTIVATED */
            // }
            // else
            // {
            //     /* To do: CARRIER_DEACTIVATED */
            // }
            // if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            // {
            //     /* DTE_ACTIVATED */
            //     if (1 == vcomInstance->attach)
            //     {
            //         vcomInstance->startTransactions = 1;
            //     }
            // }
            // else
            // {
            //     /* DTE_DEACTIVATED */
            //     if (1 == vcomInstance->attach)
            //     {
            //         vcomInstance->startTransactions = 0;
            //     }
            // }
        }

            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_SEND_BREAK:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief Virtual COM device set configuration function.
 *
 * This function sets configuration for CDC class.
 *
 * @param handle The CDC ACM class handle.
 * @param configure The CDC ACM class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomSetConfigure(usb_device_handle handle, uint8_t configure)
{
    usb_device_endpoint_init_struct_t epInitStruct;
    usb_device_endpoint_callback_struct_t epCallback;
    usb_status_t error = kStatus_USB_Error;

    if (g_composite_p->currentConfiguration == configure)
    {
        return error;
    }
    if (g_composite_p->currentConfiguration)
    {
        for (uint8_t i = 0; i < USB_DEVICE_CONFIG_CDC_ACM; i++)
        {
            USB_DeviceDeinitEndpoint(g_composite_p->deviceHandle,
                                     ((g_composite_p->cdcVcom[i].interruptEndpoint) |
                                      (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)));
            USB_DeviceDeinitEndpoint(g_composite_p->deviceHandle,
                                     ((g_composite_p->cdcVcom[0].bulkInEndpoint) |
                                      (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)));
            USB_DeviceDeinitEndpoint(g_composite_p->deviceHandle,
                                     ((g_composite_p->cdcVcom[0].bulkOutEndpoint) |
                                      (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)));
        }
    }
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        /* Initiailize cdc 1 endpoint*/
        g_composite_p->cdcVcom[0].attach = 1;

        /* Initiailize endpoint for interrupt pipe */
        epCallback.callbackFn = USB_DeviceCdcAcmInterruptIn;
        epCallback.callbackParam = (void *)&g_composite_p->cdcVcom[0].communicationInterfaceNumber;

        epInitStruct.zlt = 0;
        epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_composite_p->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
            epInitStruct.interval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
            epInitStruct.interval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
        }
        g_composite_p->cdcVcom[0].interruptEndpoint = USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT;
        g_composite_p->cdcVcom[0].interruptEndpointMaxPacketSize = epInitStruct.maxPacketSize;
        g_composite_p->cdcVcom[0].communicationInterfaceNumber = USB_CDC_VCOM_CIC_INTERFACE_INDEX;
        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        /* Initiailize endpoints for bulk in pipe */
        epCallback.callbackFn = USB_DeviceCdcAcmBulkIn;
        epCallback.callbackParam = (void *)&g_composite_p->cdcVcom[0].dataInterfaceNumber;

        epInitStruct.zlt = 0;
        epInitStruct.interval = 0;
        epInitStruct.transferType = USB_ENDPOINT_BULK;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_composite_p->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE;
        }
        g_composite_p->cdcVcom[0].bulkInEndpoint = USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT;
        g_composite_p->cdcVcom[0].bulkInEndpointMaxPacketSize = epInitStruct.maxPacketSize;
        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);
        /* Initiailize endpoints for bulk out pipe */
        epCallback.callbackFn = USB_DeviceCdcAcmBulkOut;
        epCallback.callbackParam = (void *)&g_composite_p->cdcVcom[0].dataInterfaceNumber;

        epInitStruct.zlt = 0;
        epInitStruct.interval = 0;
        epInitStruct.transferType = USB_ENDPOINT_BULK;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_composite_p->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
        }
        g_composite_p->cdcVcom[0].bulkOutEndpoint = USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT;
        g_composite_p->cdcVcom[0].bulkOutEndpointMaxPacketSize = epInitStruct.maxPacketSize;
        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        g_composite_p->cdcVcom[0].dataInterfaceNumber = USB_CDC_VCOM_DIC_INTERFACE_INDEX;

        /* Schedule buffer for receive */
        USB_DeviceRecvRequest(handle, g_composite_p->cdcVcom[0].bulkOutEndpoint,
                              g_composite_p->cdcVcom[0].currRecvBuf,
                              g_composite_p->cdcVcom[0].bulkOutEndpointMaxPacketSize);

        /****** Initiailize cdc 2 endpoint*****/
        g_composite_p->cdcVcom[1].attach = 1;

        /* Initiailize endpoint for interrupt pipe */
        epCallback.callbackFn = USB_DeviceCdcAcmInterruptIn;
        epCallback.callbackParam = (void *)&g_composite_p->cdcVcom[1].communicationInterfaceNumber;

        epInitStruct.zlt = 0;
        epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2 | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_composite_p->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2;
            epInitStruct.interval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE_2;
            epInitStruct.interval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL_2;
        }
        g_composite_p->cdcVcom[1].interruptEndpoint = USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT_2;
        g_composite_p->cdcVcom[1].interruptEndpointMaxPacketSize = epInitStruct.maxPacketSize;
        g_composite_p->cdcVcom[1].communicationInterfaceNumber = USB_CDC_VCOM_CIC_INTERFACE_INDEX_2;
        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        /* Initiailize endpoints for bulk in pipe */
        epCallback.callbackFn = USB_DeviceCdcAcmBulkIn;
        epCallback.callbackParam = (void *)&g_composite_p->cdcVcom[1].dataInterfaceNumber;

        epInitStruct.zlt = 0;
        epInitStruct.interval = 0;
        epInitStruct.transferType = USB_ENDPOINT_BULK;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2 | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_composite_p->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE_2;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE_2;
        }
        g_composite_p->cdcVcom[1].bulkInEndpoint = USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT_2;
        g_composite_p->cdcVcom[1].bulkInEndpointMaxPacketSize = epInitStruct.maxPacketSize;
        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);
        /* Initiailize endpoints for bulk out pipe */
        epCallback.callbackFn = USB_DeviceCdcAcmBulkOut;
        epCallback.callbackParam = (void *)&g_composite_p->cdcVcom[1].dataInterfaceNumber;

        epInitStruct.zlt = 0;
        epInitStruct.interval = 0;
        epInitStruct.transferType = USB_ENDPOINT_BULK;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2 | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_composite_p->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE_2;
        }
        g_composite_p->cdcVcom[1].bulkOutEndpoint = USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT_2;
        g_composite_p->cdcVcom[1].bulkOutEndpointMaxPacketSize = epInitStruct.maxPacketSize;
        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        g_composite_p->cdcVcom[1].dataInterfaceNumber = USB_CDC_VCOM_DIC_INTERFACE_INDEX_2;

        /* Schedule buffer for receive */
        USB_DeviceRecvRequest(handle, g_composite_p->cdcVcom[1].bulkOutEndpoint,
                              g_composite_p->cdcVcom[1].currRecvBuf,
                              g_composite_p->cdcVcom[1].bulkOutEndpointMaxPacketSize);
    }
    return kStatus_USB_Success;
}

void USB_DeviceClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_composite_p->deviceHandle);
}

void USB_OTG2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_composite_p->deviceHandle);
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            USB_DeviceControlPipeInit(handle);
            g_composite.attach = 0;
            g_composite.currentConfiguration = 0;

            if (kStatus_USB_Success == USB_DeviceGetStatus(handle, kUSB_DeviceStatusSpeed, (void *) &g_composite.speed))
            {
                USB_DeviceSetSpeed(handle, g_composite.speed);
            }
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                g_composite.attach = 0U;
                g_composite.currentConfiguration = 0U;
            }
            else if (USB_COMPOSITE_CONFIGURE_INDEX == (*temp8))
            {
                g_composite.attach = 1;
                USB_DeviceCdcVcomSetConfigure(handle, *temp8);
                g_composite.currentConfiguration = *temp8;
                error = kStatus_USB_Success;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief Get the setup packet buffer.
 *
 * This function provides the buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setupBuffer The pointer to the address of setup packet buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    static uint32_t compositeSetup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&compositeSetup;
    return kStatus_USB_Success;
    (void) handle;
    (void) setupBuffer;
}

/*!
 * @brief Get the vendor request data buffer.
 *
 * This function gets the data buffer for vendor request.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetVendorReceiveBuffer(usb_device_handle handle,
                                              usb_setup_struct_t *setup,
                                              uint32_t *length,
                                              uint8_t **buffer)
{
    return kStatus_USB_Error;
    (void) handle;
    (void) setup;
    (void) length;
    (void) buffer;
}

/*!
 * @brief CDC vendor specific callback function.
 *
 * This function handles the CDC vendor specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceProcessVendorRequest(usb_device_handle handle,
                                            usb_setup_struct_t *setup,
                                            uint32_t *length,
                                            uint8_t **buffer)
{
    return kStatus_USB_InvalidRequest;
    (void) handle;
    (void) setup;
    (void) length;
    (void) buffer;
}

/*!
 * @brief Configure remote wakeup feature.
 *
 * This function configures the remote wakeup feature.
 *
 * @param handle The USB device handle.
 * @param enable 1: enable, 0: disable.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
    (void) handle;
    (void) enable;
}

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    error = USB_DeviceCdcVcomConfigureEndpointStatus(handle, ep, status);

    return error;
}

/*!
 * @brief Get the setup packet data buffer.
 *
 * This function gets the data buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    static uint8_t setupOut[8];
    if ((NULL == buffer) || ((*length) > sizeof(setupOut)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = setupOut;
    return kStatus_USB_Success;
    (void) handle;
    (void) setup;
}

/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    return USB_DeviceCdcVcomClassRequest(handle, setup, length, buffer);
}

/*!
 * @brief Virtual COM device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param*g_composite_p The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomInit(usb_device_composite_struct_t *deviceComposite)
{
    for (uint8_t i = 0; i < USB_DEVICE_CONFIG_CDC_ACM; i++)
    {
        deviceComposite->cdcVcom[i].lineCoding = (uint8_t *)&s_lineCoding[i];
        deviceComposite->cdcVcom[i].abstractState = (uint8_t *)&s_abstractState[i];
        deviceComposite->cdcVcom[i].countryCode = (uint8_t *)&s_countryCode[i];
        deviceComposite->cdcVcom[i].usbCdcAcmInfo = &s_usbCdcAcmInfo[i];
        deviceComposite->cdcVcom[i].currRecvBuf = (uint8_t *)&s_cdc_data.s_currRecvBuf[i][0];
        deviceComposite->cdcVcom[i].currSendBuf = (uint8_t *)&s_cdc_data.s_currSendBuf[i][0];
    }
    return kStatus_USB_Success;
}


#ifdef USB_CONSOLE_DEBUG

void vcom_debug_thread(void * argument)
{
    /* For future implementation of reading USB debug console. */

    s_cdc_data.xReadToNotify[1] = xTaskGetCurrentTaskHandle();
    while(1)
    {
        xTaskNotifyWait(0x0, 0x0, NULL, portMAX_DELAY);
    }
    (void) argument;
}

#endif
