
#include "fsl_clock.h"
#include "fsl_flexspi.h"
#include "fsl_gpio.h"
#include <stdint.h>

const uint32_t HYPERRAM_SIZE = (8 * 1024 * 1024);

#define HYPERRAM_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READREG 2
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG 3

static const uint32_t customLUT[20] = {
    /* Read Data */
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0,
                        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10,
                        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04,
                        kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

    /* Write Data */
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x20,
                        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10,
                        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04,
                        kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

    /* Read Register */
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD,
                        0xE0, // Send first byte 0xE0 to device
                        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD,
                        0x18), // Send 0x18=24bits with Row Address to device
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD,
                        0x10, // Send 0x10=16bits witch Column Address to device
                        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD,
                        0x06), // Wait dummy bytes 0x06 = 6 times
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 2] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, // Read from device
        kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

    /* Write Register */
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60,
                        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10,
                        kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
    [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04,
                        kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),
};

static void delay(void) {
  for (volatile uint32_t i = 0; i < 1000000; ++i) {
    __asm("NOP");
  }
}

void BOARD_InitHyperRAM(void) {
  flexspi_config_t flexspi_config;
  uint32_t hyperBusDiv = CLOCK_GetDiv(kCLOCK_Flexspi2Div) + 1;
  uint32_t hyperBusClk = CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk) / hyperBusDiv;

  // Configure HyperRAM reset pin
  const gpio_pin_config_t resetpin_config = {.direction = kGPIO_DigitalOutput,
                                             .outputLogic = 0,
                                             .interruptMode = kGPIO_NoIntmode};

  GPIO_PinInit(GPIO4, 11, &resetpin_config);

  // Reset HyperRAM
  GPIO_WritePinOutput(GPIO4, 11, 0);
  delay();
  GPIO_WritePinOutput(GPIO4, 11, 1);
  delay();

  /* Get FLEXSPI default settings and configure the flexspi. */
  FLEXSPI_GetDefaultConfig(&flexspi_config);

  /* Init FLEXSPI. */
  /* enable diff clock and DQS */
  flexspi_config.rxSampleClock = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
  flexspi_config.enableSckBDiffOpt = true;
  flexspi_config.enableCombination = true;
  /*Set AHB buffer size for reading data through AHB bus. */
  flexspi_config.ahbConfig.enableAHBPrefetch = true;
  /*Allow AHB read start address do not follow the alignment requirement. */
  flexspi_config.ahbConfig.enableReadAddressOpt = true;
  flexspi_config.ahbConfig.enableAHBBufferable = true;
  flexspi_config.ahbConfig.enableAHBCachable = true;
 
  for (uint8_t i = 0; i < FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT; i++) {
    flexspi_config.ahbConfig.buffer[i].enablePrefetch = false; /* Disable AHB prefetch. */
  }

  FLEXSPI_Init(FLEXSPI2, &flexspi_config);

  flexspi_device_config_t flexspi_device_config = {
      .flexspiRootClk = hyperBusClk, /* 320MHZ SPI serial clock */
      .isSck2Enabled = false,
      .flashSize = HYPERRAM_SIZE / 1024, // Memory size in kB
      .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
      .CSInterval = 2,
      .CSHoldTime = 0,
      .CSSetupTime = 4,
      .dataValidTime = 1,
      .columnspace = 3,
      .enableWordAddress = true,
      .AWRSeqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA,
      .AWRSeqNumber = 1,
      .ARDSeqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA,
      .ARDSeqNumber = 1,
      .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
      .AHBWriteWaitInterval = 0,
      .enableWriteMask = true,
  };

  /* Configure RAM settings according to serial RAM feature. */
  FLEXSPI_SetFlashConfig(FLEXSPI2, &flexspi_device_config, kFLEXSPI_PortA1);

  /* Update LUT table. */
  FLEXSPI_UpdateLUT(FLEXSPI2, 0, customLUT, ARRAY_SIZE(customLUT));

  /* Do software reset. */
  FLEXSPI_SoftwareReset(FLEXSPI2);
  delay();
}