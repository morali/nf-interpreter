//
// Copyright (c) 2019 The nanoFramework project contributors
// Portions Copyright (c) 2015, Freescale Semiconductor, Inc. All rights reserved.
// Portions Copyright 2016-2017 NXP All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "fsl_phy.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the timeout macro. */
#define PHY_TIMEOUT_COUNT 0x3FFFFFFU

/* KSZ8863 PHY register offsets */
#define LAN8_BCR_REG        0x0		/*!< Basic Control Register */
#define LAN8_BSR_REG        0x1		/*!< Basic Status Reg */
#define LAN8_PHYID1_REG     0x2		/*!< PHY ID 1 Reg  */
#define LAN8_PHYID2_REG		0x3		/*!< PHY ID 2 Reg */
#define LAN8_PHYANA_REG		0x4		/*!< PHY Auto-Negotiation Advertisement Reg */
#define LAN8_PHYANLPA_REG	0x5		/*!< PHY Auto-Negotiation Link Partner Ability Reg */
#define LAN8_PHYLCS			0x1D	/*!< PHY LinkMD Control/Status */
#define LAN8_PHYSCS_REG		0x1F	/*!< PHY Special Control/Status Reg  */

/* KSZ8863 BCR register definitions */
#define LAN8_RESET          (1 << 15)	/*!< 1= S/W Reset */
#define LAN8_LOOPBACK       (1 << 14)	/*!< 1=loopback Enabled */
#define LAN8_SPEED_SELECT   (1 << 13)	/*!< 1=Select 100MBps */
#define LAN8_AUTONEG        (1 << 12)	/*!< 1=Enable auto-negotiation */
#define LAN8_POWER_DOWN     (1 << 11)	/*!< 1=Power down PHY */
#define LAN8_ISOLATE        (1 << 10)	/*!< 1=Isolate PHY */
#define LAN8_RESTART_AUTONEG (1 << 9)	/*!< 1=Restart auto-negoatiation */
#define LAN8_DUPLEX_MODE    (1 << 8)	/*!< 1=Full duplex mode */

/* KSZ8863 BSR register definitions */
#define LAN8_100BASE_T4     (1 << 15)	/*!< T4 mode */
#define LAN8_100BASE_TX_FD  (1 << 14)	/*!< 100MBps full duplex */
#define LAN8_100BASE_TX_HD  (1 << 13)	/*!< 100MBps half duplex */
#define LAN8_10BASE_T_FD    (1 << 12)	/*!< 100Bps full duplex */
#define LAN8_10BASE_T_HD    (1 << 11)	/*!< 10MBps half duplex */
#define LAN8_AUTONEG_COMP   (1 << 5)	/*!< Auto-negotation complete */
#define LAN8_RMT_FAULT      (1 << 4)	/*!< Fault */
#define LAN8_AUTONEG_ABILITY (1 << 3)	/*!< Auto-negotation supported */
#define LAN8_LINK_STATUS    (1 << 2)	/*!< 1=Link active */
#define LAN8_JABBER_DETECT  (1 << 1)	/*!< Jabber detect */
#define LAN8_EXTEND_CAPAB   (1 << 0)	/*!< Supports extended capabilities */

/* KSZ8863 PHYSPLCTL status definitions */
#define LAN8_SPEEDMASK      (15 << 5)	/*!< Speed and duplex mask */
#define LAN8_SPEED100F      (8 << 5)	/*!< 100BT full duplex */
#define LAN8_SPEED10F       (2 << 5)	/*!< 10BT full duplex */
#define LAN8_SPEED100H      (4 << 5)	/*!< 100BT half duplex */
#define LAN8_SPEED10H       (1 << 5)	/*!< 10BT half duplex */

/* KSZ8863 PHY ID 1/2 register definitions */
#define LAN8_PHYID1_OUI     0x0022		/*!< Expected PHY ID1 */
#define LAN8_PHYID2_OUI     0x1430		/*!< Expected PHY ID2, except last 4 bits */


enum{
	port1 = 1,
	port2,
	port3
};

#define GLOBAL_REGISTERS_BASE_ADDR	0

#define PORT1_BASE_ADDR			16
#define PORT2_BASE_ADDR			32
#define PORT3_BASE_ADDR			48


#define REG_CHIP_ID0			0
#define REG_CHIP_ID1_SWITCH		1
#define REG_GLOBAL_CTRL0		2
#define REG_GLOBAL_CTRL1		3
#define REG_GLOBAL_CTRL2		4
#define REG_GLOBAL_CTRL3		5

#define REG_GLOBAL_CTRL4		6
#define REG_GLOBAL_CTRL5		7
#define REG_GLOBAL_CTRL6		8
#define REG_GLOBAL_CTRL7		9
#define REG_GLOBAL_CTRL8		10
#define REG_GLOBAL_CTRL9		11
#define REG_GLOBAL_CTRL10		12
#define REG_GLOBAL_CTRL11		13
#define REG_GLOBAL_CTRL12		14
#define REG_GLOBAL_CTRL13		15


#define REG_CTRL0				0
#define REG_CTRL1				1
#define REG_CTRL2				2
#define REG_CTRL3				3
#define REG_CTRL4				4
#define REG_CTRL5				5
#define REG_Q0_IDRL				6
#define REG_Q1_IDRL				7
#define REG_Q2_IDRL				8
#define REG_Q3_IDRL				9
#define REG_PHY_SPCL_CTRL_STAT	10			//NOT APPLIED TO PORT 3
#define REG_LINKMD_RES			11			//ONLY PORT 2
#define REG_CTRL12				12			//NOT APPLIED TO PORT 3
#define REG_CTRL13				13			//NOT APPLIED TO PORT 3

#define REG_STAT0				14			//NOT APPLIED TO PORT 3
#define REG_STAT1				15

#define REG_RESET				67

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the ENET instance from peripheral base address.
 *
 * @param base ENET peripheral base address.
 * @return ENET instance.
 */
extern uint32_t ENET_GetInstance(ENET_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to enet clocks for each instance. */
extern clock_ip_name_t s_enetClock[FSL_FEATURE_SOC_ENET_COUNT];
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t PHY_Init(ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz)
{

    uint32_t counter = PHY_TIMEOUT_COUNT;
    uint32_t idReg = 0;
    uint32_t instance = ENET_GetInstance(base);
    status_t result = kStatus_Success;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Set SMI first. */
    CLOCK_EnableClock(s_enetClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    ENET_SetSMI(base, srcClock_Hz, false);

    /* Initialization after PHY stars to work. */
    while ((idReg != PHY_CONTROL_ID1) && (counter != 0))
    {
        PHY_Read(base, phyAddr, PHY_ID1_REG, &idReg);
        counter --;
    }

    if (!counter)
    {
        return kStatus_Fail;
    }


	uint32_t tmp;

	/* Put the KSZ8863 in reset mode and wait for completion */
	//TODO zostalo z AAC20 ale z tego co widze w DOC od KSZ to to nie jest wspierane ???
	result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, PHY_BCTL_RESET_MASK);
	if (result != kStatus_Success)
	{
		return kStatus_Fail;
	}

	counter = PHY_TIMEOUT_COUNT;
	while (counter--) {

		result = PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &tmp);
		if (result != kStatus_Success)
		{
			return kStatus_Fail;
		}
		if (!(tmp & (PHY_BCTL_RESET_MASK | LAN8_POWER_DOWN)))
		{
			break;
		}

	}
	/* Timeout? */
	if (counter == 0)
	{
		 return kStatus_Fail;
	}

	/* Setup link */
	PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, LAN8_AUTONEG);

	/* The link is not set active at this point, but will be detected
	   later */


    return kStatus_Success;
}

status_t PHY_Write(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t data)
{
    uint32_t counter;

    /* Clear the SMI interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI write command. */
    ENET_StartSMIWrite(base, phyAddr, phyReg, kENET_MiiWriteValidFrame, data);

    /* Wait for SMI complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_Read(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t *dataPtr)
{
    assert(dataPtr);

    uint32_t counter;

    /* Clear the MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI read command operation. */
    ENET_StartSMIRead(base, phyAddr, phyReg, kENET_MiiReadValidFrame);

    /* Wait for MII complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Get data from MII register. */
    *dataPtr = ENET_ReadSMIData(base);

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_SmiRegRead(ENET_Type *base, uint8_t reg, uint8_t *data)
{
    assert(data);

    uint32_t counter;

    /* Clear the MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI read command witch Read/Write OP Code = 0 */
    //ENET_StartSMIRead(base, ((reg >> 5)&0x07) | 0b10000, reg & 0x1F, kENET_MiiWriteNoCompliant);
    ENET_StartSMIWrite(base, ((reg >> 5)&0x07) | 0b10000, reg & 0x1F, kENET_MiiWriteNoCompliant, 0xFF);

    /* Wait for MII complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Get data from MII register. */
    uint32_t value = ENET_ReadSMIData(base);
    *data = (uint8_t)value;

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}



status_t PHY_SmiRegWrite(ENET_Type *base, uint8_t reg, uint8_t data)
{
    uint32_t counter;

    /* Clear the SMI interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI write command witch Read/Write OP Code = 0 */
    ENET_StartSMIWrite(base, ((reg >> 5)&0x07) | 0x0000, reg & 0x1F, kENET_MiiWriteNoCompliant, data);

    /* Wait for SMI complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_EnableLoopback(ENET_Type *base, uint32_t phyAddr, phy_loop_t mode, phy_speed_t speed, bool enable)
{
    status_t result;
    uint32_t data = 0;

    /* Set the loop mode. */
    if (enable)
    {
        if (mode == kPHY_LocalLoop)
        {
            if (speed == kPHY_Speed100M)
            {
                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            else
            {
                data = PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
           return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);
        }
        else
        {
            /* First read the current status in control register. */
            result = PHY_Read(base, phyAddr, PHY_CONTROL2_REG, &data);
            if (result == kStatus_Success)
            {
                return PHY_Write(base, phyAddr, PHY_CONTROL2_REG, (data | PHY_CTL2_REMOTELOOP_MASK));
            }
        }
    }
    else
    {
        /* Disable the loop mode. */
        if (mode == kPHY_LocalLoop)
        {
            /* First read the current status in control register. */
            result = PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &data);
            if (result == kStatus_Success)
            {
                data &= ~PHY_BCTL_LOOP_MASK;
                return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, (data | PHY_BCTL_RESTART_AUTONEG_MASK));
            }
        }
        else
        {
            /* First read the current status in control one register. */
            result = PHY_Read(base, phyAddr, PHY_CONTROL2_REG, &data);
            if (result == kStatus_Success)
            {
                return PHY_Write(base, phyAddr, PHY_CONTROL2_REG, (data & ~PHY_CTL2_REMOTELOOP_MASK));
            }
        }
    }
    return result;
}

status_t PHY_GetLinkStatus(ENET_Type *base, uint32_t phyAddr, bool *status)
{
    assert(status);

    status_t result = kStatus_Success;
    uint32_t data;

    /* Read the basic status register. */
    result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &data);
    if (result == kStatus_Success)
    {
        if (!(PHY_BSTATUS_LINKSTATUS_MASK & data))
        {
            /* link down. */
            *status = false;
        }
        else
        {
            /* link up. */
            *status = true;
        }
    }
    return result;
}

status_t PHY_GetLinkSpeedDuplex(ENET_Type *base, uint32_t phyAddr, phy_speed_t *speed, phy_duplex_t *duplex)
{
    assert(duplex);

    status_t result = kStatus_Success;
    uint32_t data, ctlReg;

    /* Read the control two register. */
    result = PHY_Read(base, phyAddr, PHY_CONTROL1_REG, &ctlReg);
    if (result == kStatus_Success)
    {
        data = ctlReg & PHY_CTL1_SPEEDUPLX_MASK;
        if ((PHY_CTL1_10FULLDUPLEX_MASK == data) || (PHY_CTL1_100FULLDUPLEX_MASK == data))
        {
            /* Full duplex. */
            *duplex = kPHY_FullDuplex;
        }
        else
        {
            /* Half duplex. */
            *duplex = kPHY_HalfDuplex;
        }

        data = ctlReg & PHY_CTL1_SPEEDUPLX_MASK;
        if ((PHY_CTL1_100HALFDUPLEX_MASK == data) || (PHY_CTL1_100FULLDUPLEX_MASK == data))
        {
            /* 100M speed. */
            *speed = kPHY_Speed100M;
        }
        else
        { /* 10M speed. */
            *speed = kPHY_Speed10M;
        }
    }

    return result;
}