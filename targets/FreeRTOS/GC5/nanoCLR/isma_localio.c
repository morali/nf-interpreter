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
local_io_tasks_t local_io_tasks;

extern spi_t s_spi3;

void vLocalIOThread(void *argument) {
	(void)argument;

    // xTaskCreate(vTimeredTask10ms, "vTimeredTask10ms", configMINIMAL_STACK_SIZE, NULL, 3, &local_io_tasks.Task10ms);

	/* LocalIO UI task */
	xTaskCreate(vLocalIO_UI, "vLocalIO_UI", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

	/* Initialiaze SPI data structer to default values */
	s_local_io_tx.digital_output = 0xFF;
	s_local_io_tx.analog_output = 0x00;
	s_local_io_tx.ui_input = 0x00;
	/* Send and receive data through loopback  */
	s_spi3.spi_transfer.txData = &(s_local_io_tx.ui_input);

	/* Peripheral interrupt timer init (for SPI ring transfer and PWM) */
	PITChannel0Init();
	vTaskDelete(NULL);
}

/**
 * @brief  Low priority task fired every 1s (used for driving DO)
 * @note
 * @param  argument: (void)
 * @retval None
 */
void vTimeredTask1s(void * argument)
{
	(void) argument;

	while(1)
	{
		/* Wait for unblock from timer interrupt */
		ulTaskNotifyTake(0x0, portMAX_DELAY);

		/* Toggle digital outputs every 1s */
		s_local_io_tx.digital_output ^= 0xFF;
	}
}

#ifdef __cplusplus
}
#endif