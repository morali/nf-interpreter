
#include <nanoPAL_COM.h>

/* Initialize USB port during nanoCLR initialization. */
bool DebuggerPort_Initialize(COM_HANDLE comPortNum)
{
    /* For now we use only USB driver */
    /* TODO: implement serial port driver or SWO */

    NATIVE_PROFILE_PAL_COM();

    return true;
    (void)comPortNum;
}

/* Write all incoming debug messages to USB port. */
uint32_t GenericPort_Write( int portNum, const char* data, size_t size )
{
    /* For now we use only hardcoded USB driver */
    /* TODO: implement serial port or SWO */

    NATIVE_PROFILE_PAL_COM();
   

    return 0;
    
    (void) size;
    (void) data;
    (void) portNum;
}