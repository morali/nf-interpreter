//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "lwip/opt.h"

#include "nf_lwipthread.h"

#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "netif/etharp.h"

#include "ethernetif.h"
#include "board.h"

#include "lwip/apps/mdns.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x02U) /* Phy address of enet port 0. */

static struct netif thisif = { 0 };

struct netif * nf_getNetif() {
    return &thisif;
}

static void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

static void stack_init(const lwipthread_opts_t *opts)
{
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = BOARD_ENET0_PHY_ADDRESS, .clockName = kCLOCK_CoreSysClk
    };

    memcpy(fsl_enet_config0.macAddress,opts->macaddress,NETIF_MAX_HWADDR_LEN);

    tcpip_init(NULL, NULL);

    fsl_netif0_ipaddr.addr = opts->address;
    fsl_netif0_netmask.addr = opts->netmask;
    fsl_netif0_gw.addr = opts->gateway;

    netifapi_netif_add(&thisif, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0,
                       ethernetif0_init, tcpip_input);
    netifapi_netif_set_default(&thisif);
    netifapi_netif_set_up(&thisif);

    //mdns_resp_init();
    //mdns_resp_add_netif(&thisif, opts->ourHostName, 60);
    // mdns_resp_add_service(&thisif, MDNS_HOSTNAME, "_http", DNSSD_PROTO_TCP, 80, 300, http_srv_txt, NULL);

    // LWIP_PLATFORM_DIAG(("\r\n************************************************"));
    // LWIP_PLATFORM_DIAG((" HTTP Server example"));
    // LWIP_PLATFORM_DIAG(("************************************************"));
    // LWIP_PLATFORM_DIAG((" IPv4 Address     : %u.%u.%u.%u", ((u8_t *)&fsl_netif0_ipaddr)[0],
    //                     ((u8_t *)&fsl_netif0_ipaddr)[1], ((u8_t *)&fsl_netif0_ipaddr)[2],
    //                     ((u8_t *)&fsl_netif0_ipaddr)[3]));
    // LWIP_PLATFORM_DIAG((" IPv4 Subnet mask : %u.%u.%u.%u", ((u8_t *)&fsl_netif0_netmask)[0],
    //                     ((u8_t *)&fsl_netif0_netmask)[1], ((u8_t *)&fsl_netif0_netmask)[2],
    //                     ((u8_t *)&fsl_netif0_netmask)[3]));
    // LWIP_PLATFORM_DIAG((" IPv4 Gateway     : %u.%u.%u.%u", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
    //                     ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]));
    // LWIP_PLATFORM_DIAG((" mDNS hostname    : %s", MDNS_HOSTNAME));
    // LWIP_PLATFORM_DIAG(("************************************************"));
}

void lwipInit(const lwipthread_opts_t *opts) {

    BOARD_InitModuleClock();

    //enable source reference clock for CPU
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, false);
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1RefClkMode, true);

    stack_init(opts);

}