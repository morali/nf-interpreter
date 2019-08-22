/*
 * Created on Thu Aug 22 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_localio.h"
#include "localIO_UI.h"
#include "i2c.h"

const char *DO_Ouputs[] = {
  [0] = "DO1", 
  [1] = "DO2", 
  [2] = "DO3", 
  [3] = "DO4",
  [4] = "DO5"
};

local_io_t local_io_rx;
local_io_t local_io_tx;

lpspi_rtos_handle_t lpspi3_master_rtos_handle;

uint32_t GetDONumber()
{
    return DO_OutputsNo;
}

const char *GetDOName(uint32_t DONumber) {
  if (DONumber >= DO_OutputsNo) {
    return NULL;
  }
  return DO_Ouputs[DONumber];
}

/**
 * @brief Checks digital output bit and returns its state
 * @note   
 * @param  DONum: digital output port number
 * @retval true - high, false - low
 */
bool GetDO(uint32_t DONum)
{	
	if (DONum > DO_OutputsNo) return false;

	bool state = false;
	state = (bool) (local_io_tx.digital_output >> (DONum +3)) & 1U;
	return state;
}

/**
 * @brief  Sets digital output bit state
 * @note   
 * @param  state: true or false (high or low)
 * @param  DONum: digital output port number
 * @retval None
 */
void SetDO(bool state, uint32_t DONum)
{
	if (DONum > DO_OutputsNo) return;
		
	if (state)
	{
		local_io_tx.digital_output |= 1U << (DONum + 3);
	}
	else
	{
		local_io_tx.digital_output &= ~(1U << (DONum + 3));
	}
}

/**
 * @brief  Toggle state of the digital output port of the UAC18
 * @note   
 * @param  DONum: digital output port number
 * @retval returns state of port after toggle
 */
bool ToggleDO(uint32_t DONum)
{
	if (DONum > DO_OutputsNo) return false;

	local_io_tx.digital_output ^= 1U << (DONum + 3);

	bool state = false;
	/* Check DONum bit state and return the value */
	state = (bool) (local_io_tx.digital_output >> (DONum +3)) & 1U;
	return state;
}

void vLocalIOThread(void * argument)
{
	(void) argument;

	local_io_tx.digital_output = 0x00;
	local_io_tx.analog_output = 0x00;
	local_io_tx.ui_input = 0x00;

    lpspi_transfer_t SPI3MasterXfer = {0};

    /* Send and receive data through loopback  */
    SPI3MasterXfer.txData = &(local_io_tx.ui_input);

    SPI3MasterXfer.rxData = &(local_io_rx.ui_input);
    SPI3MasterXfer.dataSize = 3; 

	I2C2_InitPeripheral();

	xTaskCreate(vLocalIO_UI, "vLocalIO_UI", configMINIMAL_STACK_SIZE, NULL, uxTaskPriorityGet(NULL), NULL);

	while(1)
	{
		LPSPI_RTOS_Transfer(&s_spi3.masterRtosHandle, &SPI3MasterXfer);			
		
		GPIO_WritePinOutput(GPIO1, 28, 1);
		vTaskDelay(pdMS_TO_TICKS(1));
		GPIO_WritePinOutput(GPIO1, 28, 0);
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}



