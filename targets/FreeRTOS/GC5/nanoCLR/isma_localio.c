
#include "isma_localio.h"

lpspi_rtos_handle_t lpspi3_master_rtos_handle;

void SPI3Init()
{
    local_io_t local_io_rx;
    local_io_t local_io_tx;

	lpspi_master_config_t masterConfig;
    lpspi_transfer_t SPI3MasterXfer = {0};
	status_t status = 1;

    NVIC_SetPriority(LPSPI3_IRQn, LPSPI3_IRQ_PRIO);

    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = LPSPI3_BAUDRATE;

    LPSPI_RTOS_Init(&lpspi3_master_rtos_handle, LPSPI3, &masterConfig, LPSPI3_CLOCK_FREQUENCY);

    /* Send and receive data through loopback  */
    SPI3MasterXfer.txData = &(local_io_tx.ui_input);
    SPI3MasterXfer.rxData = &(local_io_rx.ui_input);
    SPI3MasterXfer.dataSize = 3; 

	/* Inicjalizacja pinu CS GPIO1.8 */
	const gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    GPIO_PinInit(GPIO1, 28, &gpio_config);

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
    configASSERT(status == kStatus_Success);
}

