
#include <nanoPAL_COM.h>

#include "nanoUSB.h"

#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"

extern usb_data_t s_cdc_data;
extern usb_device_composite_struct_t * g_composite_p;

bool WP_Port_Intitialised = false;

static lpuart_rtos_handle_t handle;
static struct _lpuart_handle t_handle;
uint8_t background_buffer[0x1500];

lpuart_rtos_config_t lpuart_config = {
    .baudrate = 115200,
    .parity = kLPUART_ParityDisabled,
    .stopbits = kLPUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
    };

bool WP_Initialise()
{
 
    NVIC_SetPriority(BOARD_UART_IRQ, 2);

    lpuart_config.srcclk = BOARD_DebugConsoleSrcFreq();
    lpuart_config.base = LPUART8;

    int ret = LPUART_RTOS_Init(&handle, &t_handle, &lpuart_config);
    WP_Port_Intitialised = (ret == 0);
 
    if (!WP_Port_Intitialised) {
        vTaskSuspend(NULL);
    }
    
    return WP_Port_Intitialised;
}

/* Initialize USB port during nanoCLR initialization. */
bool DebuggerPort_Initialize(COM_HANDLE comPortNum)
{
    /* For now we use only USB driver */
    /* TODO: implement serial port driver or SWO */

    NATIVE_PROFILE_PAL_COM();

    if (!s_cdc_data.initialized)
    {
        if(USB_Init() == kStatus_USB_Success)
        {
            s_cdc_data.initialized = 1;
            return true;
        }
        return false;
    }

    return true;
    (void)comPortNum;
}

/* Write all incoming debug messages to USB port. */
uint32_t GenericPort_Write( int portNum, const char* data, size_t size )
{
    /* For now we use only hardcoded USB driver */
    /* TODO: implement serial port or SWO */

    NATIVE_PROFILE_PAL_COM();

    if(!DebuggerPort_Initialize(0)) return 0;
    
    if (!WP_Port_Intitialised) WP_Initialise();

    LPUART_RTOS_Send(&handle, (uint8_t *)data, size);

    if(!DebuggerPort_Initialize(0)) return 0;
    return (uint32_t)size;
    (void)portNum;
}