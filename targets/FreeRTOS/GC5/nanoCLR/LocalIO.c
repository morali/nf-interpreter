/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "LocalIO.h"
#include "LocalIO_AO_TO.h"
#include "LocalIO_DI.h"
#include "LocalIO_DO.h"
#include "LocalIO_Timers.h"
#include "LocalIO_UI.h"
#include "spi.h"

static local_io_t s_local_io_tx;

local_io_t *GetLocalIoTx() {
  return &s_local_io_tx;
}

void vLocalIOThread(void *argument) {
  (void)argument;

  InitDI();
  InitDO();

  /* LocalIO UI task */
  xTaskCreate(vLocalIO_UI, "vLocalIO_UI", configMINIMAL_STACK_SIZE + 30, NULL, uxTaskPriorityGet(NULL), NULL);

  /* Initialiaze SPI data structre to default values */
  s_local_io_tx.digital_output = 0xFF;
  s_local_io_tx.analog_output = 0x00;
  s_local_io_tx.ui_input = 0x00;

  lpspi_transfer_t *spi3_transfer = GetSpi3Transfer();

  spi3_transfer->txData = (uint8_t *)&s_local_io_tx;
  spi3_transfer->dataSize = 3;
  spi3_transfer->configFlags = kLPSPI_MasterByteSwap;

  /* Peripheral interrupt timer init (for SPI ring transfer and PWM) */
  PITChannel0Init();
  LPSPI_MasterTransferNonBlocking(LPSPI3, GetSpi3MasterHandle(), spi3_transfer);

  vTaskDelete(NULL);
}
