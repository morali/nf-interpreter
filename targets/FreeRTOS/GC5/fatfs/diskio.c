//
// Copyright (c) 2019 The nanoFramework project contributors
// Portions Copyright (c) 2015, Freescale Semiconductor, Inc.
// Portions Copyright 2016-2018 NXP
// See LICENSE file in the project root for full license information.
//

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"     /* Declarations of disk functions */
#include "fsl_sd_disk.h"
#include "External_RTC.h"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

    stat = sd_disk_status(pdrv);
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	stat = sd_disk_initialize(pdrv);
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	res = sd_disk_read(pdrv, buff, sector, count);

    return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	res = sd_disk_write(pdrv, buff, sector, count);

	return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	res = sd_disk_ioctl(pdrv, cmd, buff);

	return res;
}

DWORD get_fattime(void)
{
    struct tm *tmr = RTC_ReadTime();
    int year = (tmr->tm_year < 80) ? 0 : tmr->tm_year - 80;
    return    ((DWORD)(year) << 25)
            | ((DWORD)(tmr->tm_mon + 1) << 21)
            | ((DWORD)tmr->tm_mday << 16)
            | (WORD)(tmr->tm_hour << 11)
            | (WORD)(tmr->tm_min << 5)
            | (WORD)(tmr->tm_sec >> 1);
}