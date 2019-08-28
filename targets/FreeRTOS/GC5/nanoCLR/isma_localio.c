/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_localio.h"

#include "localIO_UI.h"

#include "LocalIO_Timers.h"


local_io_t local_io_tx;
local_io_tasks_t local_io_tasks;

extern spi_t s_spi3;

void vLocalIOThread(void *argument) {
	(void)argument;
	
	const gpio_pin_config_t gpioConfig = {
		.direction        = kGPIO_DigitalOutput,
		.outputLogic      = 0,
		.interruptMode    = kGPIO_NoIntmode
		};

	GPIO_PinInit(GPIO1, 28, &gpioConfig);  // LPSPI2_CS0 
	
	xTaskCreate(vTimeredTask10us, "vTimeredTask10us", configMINIMAL_STACK_SIZE, NULL, 4, &local_io_tasks.Task10us);	
    xTaskCreate(vTimeredTask10ms, "vTimeredTask10ms", configMINIMAL_STACK_SIZE, NULL, 4, &local_io_tasks.Task10ms);
	// xTaskCreate(vLocalIO_UI, "vLocalIO_UI", configMINIMAL_STACK_SIZE, NULL, 4, NULL);	    

	PITChannel0Init();

	vTaskDelete(NULL);
}

/**
 * @brief  Low priority task fired every 10ms (used for driving DO)
 * @note   
 * @param  argument: (void)
 * @retval None
 */
void vTimeredTask10ms(void * argument)
{
	(void) argument;
	
	local_io_tx.digital_output = 0x00;
	local_io_tx.analog_output = 0x00;
	local_io_tx.ui_input = 0x00;

	/* Send and receive data through loopback  */
	s_spi3.spi_transfer.txData = &(local_io_tx.ui_input);
	s_spi3.spi_transfer.dataSize = 3;
		
	while(1)
	{

		/* Wait for unblock from timer interrupt */
		ulTaskNotifyTake(0x0, portMAX_DELAY);
		
		/* Test output toggle */ 
		local_io_tx.digital_output ^= 1U << (7);

		LPSPI_RTOS_Transfer(&s_spi3.masterRtosHandle, &s_spi3.spi_transfer);			
		
		GPIO_WritePinOutput(GPIO1, 28, 1);
		vTaskDelay(1);
		GPIO_WritePinOutput(GPIO1, 28, 0);	
	}
}

/**
 * @brief  High priority task unblocked every 10us (needed and used for driving PWMs)
 * @note   
 * @param  argument: (void)
 * @retval None
 */
void vTimeredTask10us(void * argument)
{
	(void) argument;

	while(1)
	{
	/* Wait for unblock from timer interrupt */
		ulTaskNotifyTake(0x0, portMAX_DELAY);
	}	
}
