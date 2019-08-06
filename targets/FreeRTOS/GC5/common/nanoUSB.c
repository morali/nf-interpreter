
#include <nanoPAL_COM.h>

#include "nanoUSB.h"

extern usb_data_t s_cdc_data;
extern usb_device_composite_struct_t * g_composite_p;

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

    usb_status_t error = kStatus_USB_Error;
    usb_cdc_vcom_struct_t *vcomInstance;
    vcomInstance = &g_composite_p->cdcVcom[1];

    s_cdc_data.xWriteToNotify[1] = xTaskGetCurrentTaskHandle();

    if (size > sizeof(s_cdc_data.s_currSendBuf[1]) / sizeof(uint8_t)) return 0;
    memcpy(s_cdc_data.s_currSendBuf[1], data, size);

    error = USB_DeviceSendRequest(g_composite_p->deviceHandle, vcomInstance->bulkInEndpoint, s_cdc_data.s_currSendBuf[1], size);
    if (error != kStatus_USB_Success) return 0;

    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));

    return (uint32_t)size;
    (void)portNum;
}