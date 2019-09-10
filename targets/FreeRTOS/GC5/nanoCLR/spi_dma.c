
#include "spi_dma.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_edma.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

#include "fsl_clock.h"

#include "LocalIO.h"

#define LPSPI_TX_DMA_CHANNEL 2

static void lpspi_EdmaCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle,
                                                      status_t status,
                                                      void *userData);
void init_spi_dma()
{
    edma_handle_t lpspi_TxEdmaHandle;
    edma_handle_t lpspi_RxEdmaHandle;
    lpspi_master_edma_handle_t lpspi_EdmaHandle;

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);

    DMAMUX_Init(DMAMUX);
    DMAMUX_SetSource(DMAMUX, LPSPI_TX_DMA_CHANNEL, kDmaRequestMuxLPSPI3Tx);
    DMAMUX_EnableChannel(DMAMUX, LPSPI_TX_DMA_CHANNEL);
    DMAMUX_EnablePeriodTrigger(DMAMUX, LPSPI_TX_DMA_CHANNEL);

    /* DMA Mux init and EDMA init */
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DMA0, &edmaConfig);
    EDMA_CreateHandle(&lpspi_TxEdmaHandle, DMA0, LPSPI_TX_DMA_CHANNEL);

    LPSPI_MasterTransferCreateHandleEDMA(LPSPI3,
                                        &lpspi_EdmaHandle,
                                        lpspi_EdmaCallback,
                                        NULL,
                                        &lpspi_RxEdmaHandle,
                                        &lpspi_TxEdmaHandle);

    local_io_t tx;
    tx.digital_output = 0x00;
	tx.ui_input = 0x00;
    tx.analog_output = 0x70;

    local_io_t rx;

    lpspi_transfer_t lpspi_xfer;
    lpspi_xfer.dataSize = 3;
    lpspi_xfer.configFlags = kLPSPI_MasterPcs0;
    lpspi_xfer.rxData = &rx.digital_output;
    lpspi_xfer.txData = &tx.digital_output;

    LPSPI_MasterTransferEDMA(LPSPI3, &lpspi_EdmaHandle, &lpspi_xfer);
}

static void lpspi_EdmaCallback(LPSPI_Type *base,
                                lpspi_master_edma_handle_t *handle,
                                status_t status,
                                void *userData)
{
    (void) base;
    (void) handle;
    (void) status;
    (void) userData;
}
