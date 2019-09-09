/*
 * Created on Mon Sep 09 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOCALIO_H_
#define _ISMA_LOCALIO_H_

#include <stdint.h>

typedef struct _local_io {
  uint8_t ui_input;
  uint8_t analog_output;
  uint8_t digital_output;
} local_io_t;

/**
 * @brief  Local IO Thread
 * @note   
 * @param  *argument: 
 * @retval None
 */
void vLocalIOThread(void *argument);

/**
 * @brief  Returns pointer to LocalIo structure
 * @note   
 * @retval Pointer to LocalIo structure
 */
local_io_t *GetLocalIoTx();

#endif //_ISMA_LOCALIO_H_
