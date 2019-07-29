//
// Copyright (c) 2019 The nanoFramework project contributors
// Portions Copyright 2018 NXP. All rights reserved.
// See LICENSE file in the project root for full license information.
//

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "UAC18"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART8
#define BOARD_DEBUG_UART_INSTANCE 1U

#define BOARD_DEBUG_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()

#define BOARD_UART_IRQ LPUART8_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART8_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_USER_TP7_GPIO
#define BOARD_USER_TP7_GPIO GPIO1
#endif
#ifndef BOARD_USER_TP7_GPIO_PIN
#define BOARD_USER_TP7_GPIO_PIN (0U)
#endif
#ifndef BOARD_USER_TP6_GPIO
#define BOARD_USER_TP6_GPIO GPIO1
#endif
#ifndef BOARD_USER_TP6_GPIO_PIN
#define BOARD_USER_TP6_GPIO_PIN (1U)
#endif

/*! @brief The board flash size */
#define BOARD_FLASH_SIZE    (0x800000U)

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x02U) /* Phy address of enet port 0. */

/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#define BOARD_ARDUINO_INT_IRQ (GPIO1_INT3_IRQn)
#define BOARD_ARDUINO_I2C_IRQ (LPI2C1_IRQn)
#define BOARD_ARDUINO_I2C_INDEX (1)
#define BOARD_USDHC1_BASEADDR USDHC1
#define BOARD_USDHC2_BASEADDR USDHC2
#define BOARD_USDHC_CD_GPIO_BASE GPIO3
#define BOARD_USDHC_CD_GPIO_PIN 21
#define BOARD_USDHC_CD_PORT_IRQ GPIO3_Combined_16_31_IRQn
#define BOARD_USDHC_CD_PORT_IRQ_HANDLER GPIO3_Combined_16_31_IRQHandler

#define BOARD_USDHC_CD_STATUS() (GPIO_PinRead(BOARD_USDHC_CD_GPIO_BASE, BOARD_USDHC_CD_GPIO_PIN))

#define BOARD_USDHC_CD_INTERRUPT_STATUS() (GPIO_PortGetInterruptFlags(BOARD_USDHC_CD_GPIO_BASE))
#define BOARD_USDHC_CD_CLEAR_INTERRUPT(flag) (GPIO_PortClearInterruptFlags(BOARD_USDHC_CD_GPIO_BASE, flag))

#define BOARD_USDHC_CD_GPIO_INIT()                                                          \
    {                                                                                       \
        gpio_pin_config_t sw_config = {                                                     \
            kGPIO_DigitalInput, 0, kGPIO_IntRisingOrFallingEdge,                                    \
        };                                                                                  \
        GPIO_PinInit(BOARD_USDHC_CD_GPIO_BASE, BOARD_USDHC_CD_GPIO_PIN, &sw_config);        \
        GPIO_PortEnableInterrupts(BOARD_USDHC_CD_GPIO_BASE, 1U << BOARD_USDHC_CD_GPIO_PIN); \
        GPIO_PortClearInterruptFlags(BOARD_USDHC_CD_GPIO_BASE, ~0);                         \
    }
#define BOARD_HAS_SDCARD (1U)

#define BOARD_SD_SUPPORT_180V (0U)
#define BOARD_USDHC_CARD_INSERT_CD_LEVEL (0U)

#define BOARD_USDHC_MMCCARD_POWER_CONTROL(state)
#define BOARD_USDHC_MMCCARD_POWER_CONTROL_INIT()

#define BOARD_USDHC_SDCARD_POWER_CONTROL_INIT()
#define BOARD_USDHC_SDCARD_POWER_CONTROL(state) 

#define BOARD_USDHC1_CLK_FREQ (CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) / (CLOCK_GetDiv(kCLOCK_Usdhc1Div) + 1U))
#define BOARD_USDHC2_CLK_FREQ (CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) / (CLOCK_GetDiv(kCLOCK_Usdhc2Div) + 1U))

#define BOARD_SD_HOST_BASEADDR BOARD_USDHC1_BASEADDR
#define BOARD_SD_HOST_CLK_FREQ BOARD_USDHC1_CLK_FREQ
#define BOARD_SD_HOST_IRQ USDHC1_IRQn

#define BOARD_MMC_HOST_BASEADDR BOARD_USDHC2_BASEADDR
#define BOARD_MMC_HOST_CLK_FREQ BOARD_USDHC2_CLK_FREQ
#define BOARD_MMC_HOST_IRQ USDHC2_IRQn
#define BOARD_MMC_VCCQ_SUPPLY kMMC_VoltageWindow170to195
#define BOARD_MMC_VCC_SUPPLY kMMC_VoltageWindows270to360
/* we are using the BB SD socket to DEMO the MMC example,but the
* SD socket provide 4bit bus only, so we define this macro to avoid
* 8bit data bus test
*/
#define BOARD_MMC_SUPPORT_8BIT_BUS (0U)

#define BOARD_SD_HOST_SUPPORT_SDR104_FREQ (198000000U)
#define BOARD_SD_HOST_SUPPORT_HS200_FREQ (180000000U)

/* define for SD/MMC config IO driver strength dynamic */
void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength);

/* define for enabling USB serial debugging and disable caching of dma buffers */
#define ENDIANNESS USB_LITTLE_ENDIAN
#define HAL_USE_SERIAL_USB
#define USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE (1U)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
uint32_t BOARD_DebugConsoleSrcFreq(void);

void BOARD_InitDebugConsole(void);

void BOARD_ConfigMPU(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */