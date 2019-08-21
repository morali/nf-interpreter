
#include "isma_localio.h"

const char *DO_Ouputs[] = {
  [0] = "DO1", 
  [1] = "DO2", 
  [2] = "DO3", 
  [3] = "DO4",
  [4] = "DO5"
};

// static const char *UInputs[] = {
//   [0] = "UI1",
//   [1] = "UI2",
//   [2] = "UI3",
//   [3] = "UI4"
// };

// static const char *AOOutputs[] = {
//   [0] = "UI1",
//   [1] = "UI2",
//   [2] = "UI3",
//   [3] = "UI4"
// };

// static const char *DInputs[] = {
//   [0] = "DI1",
//   [1] = "DI2",
//   [2] = "DI3",
//   [3] = "DI4"
// };

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

bool GetDO(uint32_t DONum)
{
	/* TODO: implement return of state of the port */
	return DO_Ouputs[DONum];
}
void SetDO(bool state, uint32_t DONum)
{
	/* TODO: implement setting state of the port */
	(void) state; (void) DONum;
}

void vLocalIOThread(void * argument)
{
	(void) argument;
    local_io_t local_io_rx;
    local_io_t local_io_tx;

    lpspi_transfer_t SPI3MasterXfer = {0};
	status_t status = 1;

    /* Send and receive data through loopback  */
    SPI3MasterXfer.txData = &(local_io_tx.ui_input);
    SPI3MasterXfer.rxData = &(local_io_rx.ui_input);
    SPI3MasterXfer.dataSize = 3; 

	while(1)
	{

		LPSPI_RTOS_Transfer(&s_spi3.masterRtosHandle, &SPI3MasterXfer);
	}


    /* wysłanie danych do ringa SPI i sprawdzenie czy ostrzymano to samo
    wysłanie danych bez zatrzaskiwania stanów logicznych na wyjściach */
    local_io_tx.digital_ouput = 0xAA;
    local_io_tx.analog_output = 0x55;
    local_io_tx.ui_input = 0x33;
	
    LPSPI_RTOS_Transfer(&lpspi3_master_rtos_handle, &SPI3MasterXfer);
    vTaskDelay(pdMS_TO_TICKS(1));
    LPSPI_RTOS_Transfer(&lpspi3_master_rtos_handle, &SPI3MasterXfer);
    vTaskDelay(pdMS_TO_TICKS(1));

    /* sprawdz czy to co zostało wysłane do ringa wróciło */
    status = memcmp(&(local_io_tx.ui_input), &(local_io_rx.ui_input), LPSPI3_BUFFSIZE) ? kStatus_Success : kStatus_LPSPI_Error;
    

    vTaskDelay(pdMS_TO_TICKS(90));	//dodatkowy delay bo inaczej nie drukuje komunikatów na konsole
    configASSERT(status =! kStatus_Success);
}



