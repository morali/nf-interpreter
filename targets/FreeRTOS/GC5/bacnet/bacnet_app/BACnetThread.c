/**************************************************************************
 *
 * Copyright (C) 2006 Steve Karg <skarg@users.sourceforge.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *********************************************************************/

#include "BACnetThread.h"
#include "FreeRTOS.h"
#include "MAC_address.h"
#include "task.h"

#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "address.h"
#include "apdu.h"
#include "bacdcode.h"
#include "bacdef.h"
#include "bacfile.h"
#include "client.h"
#include "config.h"
#include "datalink.h"
#include "dcc.h"
#include "device.h"
#include "dlenv.h"
#include "filename.h"
#include "getevent.h"
#include "handlers.h"
#include "iam.h"
#include "lc.h"
#include "net.h"
#include "npdu.h"
#include "tsm.h"
#include "txbuf.h"
#include "version.h"

/* include the device object */
#include "device.h"

#include "GlobalEventsFlags.h"

/** Buffer used for receiving */
static uint8_t *Rx_Buf;

/** Initialize the handlers we will utilize.
 * @see Device_Init, apdu_set_unconfirmed_handler, apdu_set_confirmed_handler
 */
static void Init_Service_Handlers(void) {
  Device_Init(NULL);
  /* we need to handle who-is to support dynamic device binding */
  apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
  apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, handler_i_am_add);

  apdu_set_unrecognized_service_handler_handler(handler_unrecognized_service);

  /* Set the handlers for any confirmed services that we support. */
  /* We must implement read property - it's required! */
  apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY, handler_read_property);
  apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE, handler_read_property_multiple);
  apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROPERTY, handler_write_property);
  apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROP_MULTIPLE, handler_write_property_multiple);
}

static void TSM_Timeout(uint8_t invoke_id) { tsm_free_invoke_id(invoke_id); }

void setBIP_port(uint16_t port) {
  if (bip_get_port() != htons(port)) {
    bip_set_port(htons(port));
  }
}

void vBACnetThread(void *parameters) {
  (void)parameters;
  xEventGroupWaitBits(xGlobalEventsFlags, EVENT_ETH_OK, pdFALSE, pdTRUE, portMAX_DELAY);
  setBIP_port(47808);
  Rx_Buf = (uint8_t *)malloc(MAX_MPDU);
  memset(Rx_Buf, 0, sizeof(uint8_t) * MAX_MPDU);

  Handler_Transmit_Buffer = (uint8_t *)malloc(MAX_PDU);
  memset(Handler_Transmit_Buffer, 0, sizeof(uint8_t) * MAX_PDU);

  tsm_init();
  tsm_set_timeout_handler(TSM_Timeout);

  BACNET_ADDRESS src = {0}; /* address where message came from */
  uint16_t pdu_len = 0;
  unsigned timeout = 100; /* milliseconds */

  /* load any static address bindings to show up
     in our device bindings list */
  address_init();
  Init_Service_Handlers();
  dlenv_init();

  /* For now IP and mask is hardcoded */
  uint8_t ip_i[] = {192, 168, 10, 100};
  uint8_t mask_i[] = {255, 255, 255, 0};

  uint8_t *ip = ip_i;
  uint8_t *mask = mask_i;

  uint32_t broadcast = 0;
  for (int i = 0; i < 4; i++) {
    broadcast |= ((ip[i] | (mask[i] ^ 0xFF)) << (8 * i));
  }

  bip_set_broadcast_addr(broadcast);

  /* broadcast an I-Am on startup */
  Send_I_Am(Handler_Transmit_Buffer);
  /* loop forever */
  while (1) {

    pdu_len = datalink_receive(&src, Rx_Buf, MAX_MPDU, timeout);
    /* process */
    if (pdu_len) {
      npdu_handler(&src, Rx_Buf, pdu_len);
    }
  }
}
