
#include "board.h"

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1U))

#define LPSPI_MASTER_IRQN (LPSPI1_IRQn)
#define LPSPI_DEALY_COUNT 0xfffffU
#define LPSPI_MASTER_DMA_MUX_BASEADDR DMAMUX
#define LPSPI_MASTER_DMA_BASEADDR DMA0
