/*
 * Panel.c
 *
 *  Created on: 09.08.2019
 *      Author: Jakub Standarski
 */

#include "Panel.h"
#include "FreeRTOSCommonHooks.h"
#include "spi.h"

///////////////////////////////////////////
//       PANEL private declarations      //
///////////////////////////////////////////
static uint8_t panelTx[2] = {0xF0}; // disable all LEDs
static uint8_t panelRx[2];

static uint8_t dipswitchState[3];

static const char *LEDs[] = {
  [0] = "ETH", 
  [1] = "COMM", 
  [2] = "ALARM", 
  [3] = "ON"
};

static const char *DIPs[] = {
  [0] = "PROTOCOL",
  [1] = "BAUD",
  [2] = "CFG"
};



/************************************************************************************************************/
/*                                                                                                          */
/*                                        PANEL PRIVATE DEFINES                                             */
/*                                                                                                          */
/************************************************************************************************************/

#define LEDsNo (sizeof(LEDs) / sizeof(LEDs[0]))
#define DIPsNo (sizeof(DIPs) / sizeof(DIPs[0]))

#define SPI2_CS_SET()           GPIO_WritePinOutput(GPIO4, 1, 1)
#define SPI2_CS_CLEAR()         GPIO_WritePinOutput(GPIO4, 1, 0)
#define SERIAL_SHIFT_ENABLE()   GPIO_WritePinOutput(GPIO1, 12, 1)
#define PARALLEL_LOAD_ENABLE()  GPIO_WritePinOutput(GPIO1, 12, 0)

#define DIPSWITCH_READS     3

/************************************************************************************************************/
/*                                                                                                          */
/*                                PANEL PRIVATE FUNCTIONS DEFINITIONS                                       */
/*                                                                                                          */
/************************************************************************************************************/

static void PanelTransferStructure_Init(lpspi_transfer_t *panelTransfer) {
  panelTransfer->txData         = panelTx;
  panelTransfer->rxData         = panelRx;
  panelTransfer->dataSize       = 2;
  panelTransfer->configFlags    = kLPSPI_MasterPcs0;
}

static void SelectDipSwitch(uint8_t dipswitchNumber) {
  // clear all bits
  panelTx[0] |= (0x07);
  // set one bit
  panelTx[0] &= ~(1 << dipswitchNumber);
}

static void CheckAndSaveDipswitchState(uint8_t dipswitchData[][DIPSWITCH_READS])
{
  for(uint8_t dipswitchNumber = 0; dipswitchNumber < DIPsNo; dipswitchNumber++)
  {
    if(dipswitchData[dipswitchNumber][0] == dipswitchData[dipswitchNumber][1] && 
       dipswitchData[dipswitchNumber][1] == dipswitchData[dipswitchNumber][2])
       {
         dipswitchState[dipswitchNumber] = dipswitchData[dipswitchNumber][0];
       }
  }    
}

/************************************************************************************************************/
/*                                                                                                          */
/*                                PANEL PUBLIC FUNCTIONS DEFINITIONS                                        */
/*                                                                                                          */
/************************************************************************************************************/

void SetLed(uint32_t ledNumber, bool state) {
  if (ledNumber >= LEDsNo) {
    return;
  }

  if (state) {
      panelTx[0] &= ~(1 << (ledNumber + 4));
      panelTx[0] &= ~(1 << (ledNumber + 4));  
      panelTx[0] &= ~(1 << (ledNumber + 4));
  } else {
      panelTx[0] |= (1 << (ledNumber + 4));
  }
}

bool GetLed(uint32_t ledNumber) {
  if (ledNumber >= LEDsNo) {
    return false;
  }

  return !((panelTx[0] & (1 << (ledNumber + 4))) == (1 << (ledNumber + 4)));
}

const char *GetLedName(uint32_t ledNumber) {
  if (ledNumber >= LEDsNo) {
    return NULL;
  }

  return LEDs[ledNumber];
}

uint32_t getLedNumber() {
  return LEDsNo;
}

uint32_t getDipswitchNumber() {
  return DIPsNo;
}

const char *GetDipswitchName(uint32_t dipswitchNumber) {
  if (dipswitchNumber >= DIPsNo) {
    return NULL;
  }
  return DIPs[dipswitchNumber];
}

uint8_t GetDipswitch(uint32_t dipswitchNumber) {
  if (dipswitchNumber >= DIPsNo) {
    return 0;
  }
  return dipswitchState[dipswitchNumber];
}

/************************************************************************************************************/
/*                                                                                                          */
/*                                           FreeRTOS TASK                                                  */
/*                                                                                                          */
/************************************************************************************************************/

void vPanelThread(void *pvParameters) {
  (void)pvParameters;

  uint8_t dipswitchData[DIPsNo][DIPSWITCH_READS];
  uint8_t readCounter   = 0;
  uint8_t dipswitchToSet  = 0;
  uint8_t dipswitchToRead = DIPsNo - 1;  // Initialized with two to ensure proper save index

  lpspi_transfer_t panelTransfer;
  PanelTransferStructure_Init(&panelTransfer);

  for (;;) {
    SelectDipSwitch(dipswitchToSet);

    SERIAL_SHIFT_ENABLE();

    LPSPI_RTOS_Transfer(&s_spi2.masterRtosHandle, &panelTransfer);

    // Copy and save dipswitch configuration from second byte
    // NOTE: during first read procedure, there is a garbage data
    dipswitchData[dipswitchToRead][readCounter] = ~panelRx[1];

    // Load dipswitch and LEDs state into data latch of second shift register
    SPI2_CS_SET(); 
    FreeRTOSDelay(1);
    SPI2_CS_CLEAR();
    FreeRTOSDelay(1);
    SPI2_CS_SET(); 
    FreeRTOSDelay(1);
    SPI2_CS_CLEAR();
    FreeRTOSDelay(1);

    // Load data with dipswitch state into data latch of first shift register
    PARALLEL_LOAD_ENABLE();

    if (++dipswitchToSet == DIPsNo) {
      dipswitchToSet = 0;
    }
    if (++dipswitchToRead == DIPsNo) {
      dipswitchToRead = 0;
      if(++readCounter == DIPSWITCH_READS) {
        readCounter = 0;
      }
    }
    CheckAndSaveDipswitchState(dipswitchData);

    FreeRTOSDelay(10);
  }
}