/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "isma_localio.h"

#include "LocalIO_AO.h"
#include "LocalIO_UI.h"
#include "LocalIO_Timers.h"

extern localIO_AO_t local_ao;

local_io_t s_local_io_tx;

extern spi_t s_spi3;

void vLocalIOThread(void *argument) {
	(void)argument;

	/* LocalIO UI task */
	xTaskCreate(vLocalIO_UI, "vLocalIO_UI", configMINIMAL_STACK_SIZE + 30, NULL, uxTaskPriorityGet(NULL), NULL);

	/* Initialiaze SPI data structer to default values */
	s_local_io_tx.digital_output = 0xFF;
	s_local_io_tx.analog_output = 0x00;
	s_local_io_tx.ui_input = 0x00;
	/* Send and receive data through loopback  */
	
	s_spi3.spi_transfer.txData = &(s_local_io_tx.ui_input);
    s_spi3.spi_transfer.dataSize = 3;

	/* Peripheral interrupt timer init (for SPI ring transfer and PWM) */
	PITChannel0Init();
	LPSPI_MasterTransferNonBlocking(LPSPI3, &s_spi3.masterHandle, &s_spi3.spi_transfer);
	vTaskDelete(NULL);
}

#ifdef __cplusplus
}
#endif