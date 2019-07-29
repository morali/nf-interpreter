/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_VCOM_H_
#define _USB_VCOM_H_ 

#include "board.h"

#include "usb.h"
#include "usb_phy.h"
#include "usb_device_config.h"
#include "usb_device.h"
#include "usb_device_descriptor.h"
#include "usb_device_dci.h"
#include "usb_device_ehci.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"

#include "stream_buffer.h"


/*******************************************************************************
* Definitions
******************************************************************************/

/* Input buffer size */
#define IN_USB_BUFFER_SIZE 0x30000

#define CONTROLLER_ID kUSB_ControllerEhci0

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
/* Currently configured line coding */
#define LINE_CODING_SIZE (0x07)
#define LINE_CODING_DTERATE (115200)
#define LINE_CODING_CHARFORMAT (0x00)
#define LINE_CODING_PARITYTYPE (0x00)
#define LINE_CODING_DATABITS (0x08)

/* Communications feature */
#define COMM_FEATURE_DATA_SIZE (0x02)
#define STATUS_ABSTRACT_STATE (0x0000)
#define COUNTRY_SETTING (0x0000)

/* Notification of serial state */
#define NOTIF_PACKET_SIZE (0x08)
#define UART_BITMAP_SIZE (0x02)
#define NOTIF_REQUEST_TYPE (0xA1)


/* Task receiving USB data */
void USBThread(void * argument);

usb_status_t USB_Init(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);

/* Define the types for application */
typedef struct _usb_cdc_vcom_struct
{
    usb_device_handle deviceHandle; /* USB device handle. */
    volatile uint8_t attach; /* A flag to indicate whether a usb device is attached. 1: attached, 0: not attached */
    uint8_t speed;           /* Speed of USB device. USB_SPEED_FULL/USB_SPEED_LOW/USB_SPEED_HIGH.                 */
    volatile uint8_t startTransactions; /* A flag to indicate whether a CDC device is ready to transmit and receive data.    */
    uint8_t currentConfiguration; /* Current configuration value. */
    uint8_t currentInterfaceAlternateSetting[USB_CDC_VCOM_INTERFACE_COUNT]; /* Current alternate setting value for each interface. */
    uint8_t hasSentState; /*!< 1: The device has primed the state in interrupt pipe, 0: Not primed the state. */
} usb_cdc_vcom_struct_t;

/* Define the information relates to abstract control model */
typedef struct _usb_cdc_acm_info
{
    uint8_t serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE]; /* Serial state buffer of the CDC device to notify the
                                                                     serial state to host. */
    bool dtePresent;          /* A flag to indicate whether DTE is present.         */
    uint16_t breakDuration;   /* Length of time in milliseconds of the break signal */
    uint8_t dteStatus;        /* Status of data terminal equipment                  */
    uint8_t currentInterface; /* Current interface index.                           */
    uint16_t uartState;       /* UART state of the CDC device.                      */
} usb_cdc_acm_info_t;

typedef struct _usb_data
{
    StreamBufferHandle_t data_in;
    StreamBufferHandle_t data_out;
    uint8_t in_buffer[0x200];
    TaskHandle_t xReadToNotify;
    TaskHandle_t xWriteToNotify;
    volatile uint32_t s_recvSize;
    volatile uint32_t s_sendSize;
} usb_data_t;

#endif /* _USB_VCOM_H_ */
