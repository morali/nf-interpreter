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

usb_cdc_vcom_struct_t s_cdcVcom;
usb_cdc_vcom_struct_t *s_cdcVcom_p;

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) usb_data_t s_cdc_data;
usb_data_t *s_cdc_data_p;

/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU, (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {(COUNTRY_SETTING >> 0U) & 0x00FFU, (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_cdc_acm_info_t s_usbCdcAcmInfo;

static uint32_t s_usbBulkMaxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;

/*******************************************************************************
 * Code
 ******************************************************************************/

usb_status_t USB_Init(void) {
  usb_status_t error = kStatus_USB_Error;

  /* Initialize data buffers for incoming data. */
  s_cdc_data.data_in = xStreamBufferCreate(STREAM_RECV_USB_BUFFER, HS_CDC_VCOM_BULK_IN_PACKET_SIZE);

  USB_DeviceClockInit();

  s_cdcVcom_p = &s_cdcVcom;
  s_cdc_data_p = &s_cdc_data;

  s_cdcVcom_p->speed = USB_SPEED_HIGH;
  s_cdcVcom_p->attach = 0;
  s_cdcVcom_p->deviceHandle = NULL;

  error = USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, (usb_device_handle *)&s_cdcVcom_p->deviceHandle);
  if (error != kStatus_USB_Success)
    return error;

  USB_DeviceIsrEnable();

  error = USB_DeviceRun(s_cdcVcom_p->deviceHandle);
  if (error != kStatus_USB_Success)
    return error;

  s_cdcVcom_p->attach = 1;
  s_cdcVcom_p->startTransactions = 1;
  s_cdc_data_p->initialized = 1;

  return kStatus_USB_Success;
}

void USB_OTG1_IRQHandler(void) { USB_DeviceEhciIsrFunction(s_cdcVcom.deviceHandle); }

void USB_OTG2_IRQHandler(void) { USB_DeviceEhciIsrFunction(s_cdcVcom.deviceHandle); }

void USB_DeviceClockInit(void) {
  usb_phy_config_struct_t phyConfig = {
      BOARD_USB_PHY_D_CAL,
      BOARD_USB_PHY_TXCAL45DP,
      BOARD_USB_PHY_TXCAL45DM,
  };

  if (CONTROLLER_ID == kUSB_ControllerEhci0) {
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
  } else {
    CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
  }
  USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}
void USB_DeviceIsrEnable(void) {
  uint8_t irqNumber;

  uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
  irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

  /* Install isr, set priority, and enable IRQ. */
  NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
  EnableIRQ((IRQn_Type)irqNumber);
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
usb_status_t USB_DeviceCdcAcmInterruptIn(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam) {
  (void)callbackParam;
  (void)handle;
  (void)message;
  usb_status_t error = kStatus_USB_Error;
  s_cdcVcom.hasSentState = 0;
  return error;
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
usb_status_t USB_DeviceCdcAcmBulkIn(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam) {
  (void)callbackParam;
  usb_status_t error = kStatus_USB_Error;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if ((message->length != 0) && (!(message->length % s_usbBulkMaxPacketSize))) {
    /* If the last packet is the size of endpoint, then send also zero-ended packet,
     ** meaning that we want to inform the host that we do not have any additional
     ** data, so it can flush the output.
     */
    USB_DeviceSendRequest(handle, USB_CDC_VCOM_BULK_IN_ENDPOINT, NULL, 0);
  } else if (1 == s_cdcVcom.startTransactions) {
    if ((message->buffer != NULL) || ((message->buffer == NULL) && (message->length == 0))) {
      vTaskNotifyGiveFromISR(s_cdc_data.xWriteToNotify, &xHigherPriorityTaskWoken);
    }
  }

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

  return error;
}

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
usb_status_t USB_DeviceCdcAcmBulkOut(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam) {
  (void)callbackParam;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  usb_status_t error = kStatus_USB_Error;

  uint32_t s_recvSize = message->length;

  if ((message != NULL) && (message->buffer != NULL) && (s_recvSize <= 512) /*&& (1 == s_cdcVcom.attach)*/ && (1 == s_cdcVcom.startTransactions)) {
    xStreamBufferSendFromISR(s_cdc_data.data_in, message->buffer, message->length, &xHigherPriorityTaskWoken);

    if (!s_recvSize) {
      /* Schedule buffer for next receive event */
      error = USB_DeviceRecvRequest(handle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, s_cdc_data_p->s_currRecvBuf, s_usbBulkMaxPacketSize);
    }
  }
  error = USB_DeviceRecvRequest(handle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, s_cdc_data_p->s_currRecvBuf, s_usbBulkMaxPacketSize);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer) {
  (void)handle;
  (void)setupBuffer;
  static uint32_t cdcVcomSetup[2];
  if (NULL == setupBuffer) {
    return kStatus_USB_InvalidParameter;
  }
  *setupBuffer = (usb_setup_struct_t *)&cdcVcomSetup;
  return kStatus_USB_Success;
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
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer) {
  (void)handle;
  (void)setup;
  static uint8_t setupOut[8];
  if ((NULL == buffer) || ((*length) > sizeof(setupOut))) {
    return kStatus_USB_InvalidRequest;
  }
  *buffer = setupOut;
  return kStatus_USB_Success;
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
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable) {
  (void)handle;
  (void)enable;
  return kStatus_USB_InvalidRequest;
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
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer) {
  usb_status_t error = kStatus_USB_InvalidRequest;

  usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
  uint32_t len;
  uint8_t *uartBitmap;
  if (setup->wIndex != USB_CDC_VCOM_COMM_INTERFACE_INDEX) {
    return error;
  }

  switch (setup->bRequest) {
  case USB_DEVICE_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:
    break;
  case USB_DEVICE_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE:
    break;
  case USB_DEVICE_CDC_REQUEST_SET_COMM_FEATURE:
    if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == setup->wValue) {
      *buffer = s_abstractState;
    } else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == setup->wValue) {
      *buffer = s_countryCode;
    } else {
    }
    error = kStatus_USB_Success;
    break;
  case USB_DEVICE_CDC_REQUEST_GET_COMM_FEATURE:
    if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == setup->wValue) {
      *buffer = s_abstractState;
      *length = COMM_FEATURE_DATA_SIZE;
    } else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == setup->wValue) {
      *buffer = s_countryCode;
      *length = COMM_FEATURE_DATA_SIZE;
    } else {
    }
    error = kStatus_USB_Success;
    break;
  case USB_DEVICE_CDC_REQUEST_CLEAR_COMM_FEATURE:
    break;
  case USB_DEVICE_CDC_REQUEST_GET_LINE_CODING:
    *buffer = s_lineCoding;
    *length = LINE_CODING_SIZE;
    error = kStatus_USB_Success;
    break;
  case USB_DEVICE_CDC_REQUEST_SET_LINE_CODING:
    *buffer = s_lineCoding;
    error = kStatus_USB_Success;
    break;
  case USB_DEVICE_CDC_REQUEST_SET_CONTROL_LINE_STATE: {
    error = kStatus_USB_Success;
    acmInfo->dteStatus = setup->wValue;
    /* activate/deactivate Tx carrier */
    if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION) {
      acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
    } else {
      acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
    }

    /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
    if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) {
      acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
    }
    /* Com port of terminal tool running on PC is closed now */
    else {
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
    if (0 == s_cdcVcom.hasSentState) {
      error = USB_DeviceSendRequest(handle, USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf, len);
      if (kStatus_USB_Success != error) {
        usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
      }
      s_cdcVcom.hasSentState = 1;
    }
    /* Update status */
    if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION) {
      /*    To do: CARRIER_ACTIVATED */
    } else {
      /* To do: CARRIER_DEACTIVATED */
    }
    if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) {
      /* DTE_ACTIVATED */
      if (1 == s_cdcVcom.attach) {
        s_cdcVcom.startTransactions = 1;
      }
    } else {
      /* DTE_DEACTIVATED */
      if (1 == s_cdcVcom.attach) {
        s_cdcVcom.startTransactions = 1;
      }
    }
  } break;
  case USB_DEVICE_CDC_REQUEST_SEND_BREAK:
    break;
  default:
    break;
  }

  return error;
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
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param) {
  usb_status_t error = kStatus_USB_Error;
  uint8_t *temp8 = (uint8_t *)param;

  switch (event) {
  case kUSB_DeviceEventBusReset: {
    USB_DeviceControlPipeInit(s_cdcVcom.deviceHandle);
    s_cdcVcom.attach = 0;
    s_cdcVcom.currentConfiguration = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
    if (kStatus_USB_Success == USB_DeviceGetStatus(s_cdcVcom.deviceHandle, kUSB_DeviceStatusSpeed, &s_cdcVcom.speed)) {
      USB_DeviceSetSpeed(handle, s_cdcVcom.speed);
    }
#endif
  } break;
  case kUSB_DeviceEventSetConfiguration:
    if (0U == (*temp8)) {
      s_cdcVcom.attach = 0;
      s_cdcVcom.currentConfiguration = 0U;
    } else if (USB_CDC_VCOM_CONFIGURE_INDEX == (*temp8)) {
      usb_device_endpoint_init_struct_t epInitStruct;
      usb_device_endpoint_callback_struct_t epCallback;

      s_cdcVcom.attach = 1;
      s_cdcVcom.currentConfiguration = *temp8;

      /* Initiailize endpoint for interrupt pipe */
      epCallback.callbackFn = USB_DeviceCdcAcmInterruptIn;
      epCallback.callbackParam = handle;

      epInitStruct.zlt = 0;
      epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
      epInitStruct.endpointAddress = USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
      if (USB_SPEED_HIGH == s_cdcVcom.speed) {
        epInitStruct.maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
        epInitStruct.interval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
      } else {
        epInitStruct.maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
        epInitStruct.interval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
      }

      USB_DeviceInitEndpoint(s_cdcVcom.deviceHandle, &epInitStruct, &epCallback);

      /* Initiailize endpoints for bulk pipe */
      epCallback.callbackFn = USB_DeviceCdcAcmBulkIn;
      epCallback.callbackParam = handle;

      epInitStruct.zlt = 0;
      epInitStruct.interval = 0U;
      epInitStruct.transferType = USB_ENDPOINT_BULK;
      epInitStruct.endpointAddress = USB_CDC_VCOM_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
      if (USB_SPEED_HIGH == s_cdcVcom.speed) {
        epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE;
      } else {
        epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE;
      }

      USB_DeviceInitEndpoint(s_cdcVcom.deviceHandle, &epInitStruct, &epCallback);

      epCallback.callbackFn = USB_DeviceCdcAcmBulkOut;
      epCallback.callbackParam = handle;

      epInitStruct.zlt = 0;
      epInitStruct.interval = 0U;
      epInitStruct.transferType = USB_ENDPOINT_BULK;
      epInitStruct.endpointAddress = USB_CDC_VCOM_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
      if (USB_SPEED_HIGH == s_cdcVcom.speed) {
        epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
      } else {
        epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
      }

      USB_DeviceInitEndpoint(s_cdcVcom.deviceHandle, &epInitStruct, &epCallback);

      if (USB_SPEED_HIGH == s_cdcVcom.speed) {
        s_usbBulkMaxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
      } else {
        s_usbBulkMaxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
      }
      /* Schedule buffer for receive */
      USB_DeviceRecvRequest(handle, USB_CDC_VCOM_BULK_OUT_ENDPOINT, s_cdc_data.s_currRecvBuf, s_usbBulkMaxPacketSize);
    } else {
      error = kStatus_USB_InvalidRequest;
    }
    break;
  default:
    break;
  }

  return error;
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
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status) {
  if (status) {
    return USB_DeviceStallEndpoint(handle, ep);
  } else {
    return USB_DeviceUnstallEndpoint(handle, ep);
  }
}