//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------
#ifndef _ISMA_BACNET_NATIVE_H_
#define _ISMA_BACNET_NATIVE_H_

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>

struct Library_isma_bacnet_native_iSMA_BACnet_PartialBacnetObject
{
    static const int FIELD___isUpdatePending = 1;
    static const int FIELD___objectIdentifier = 2;
    static const int FIELD___objectName = 3;
    static const int FIELD___objectType = 4;


    //--//

};

struct Library_isma_bacnet_native_iSMA_BACnet_Objects_DevicePBO
{
    static const int FIELD___systemStatus = 5;
    static const int FIELD___vendorName = 6;
    static const int FIELD___modelName = 7;
    static const int FIELD___firmwareRevision = 8;
    static const int FIELD___applicationSoftwareRevision = 9;
    static const int FIELD___location = 10;
    static const int FIELD___description = 11;
    static const int FIELD___protocolVersion = 12;
    static const int FIELD___protocolRevision = 13;
    static const int FIELD___vendorId = 14;
    static const int FIELD___apduTimeout = 15;
    static const int FIELD___apduRetries = 16;


    //--//

};

struct Library_isma_bacnet_native_iSMA_BACnet_FirmwareApi
{
    NANOCLR_NATIVE_DECLARE(BacnetObjectAdded___STATIC__VOID__iSMABACnetPartialBacnetObject);
    NANOCLR_NATIVE_DECLARE(BacnetObjectRemoved___STATIC__VOID__iSMABACnetPartialBacnetObject);
    NANOCLR_NATIVE_DECLARE(UpdateDatabaseRevision___STATIC__VOID);

    //--//

};

struct Library_isma_bacnet_native_iSMA_BACnet_Objects_AnalogValuePBO
{
    static const int FIELD___description = 5;
    static const int FIELD___statusFlags = 6;
    static const int FIELD___eventState = 7;
    static const int FIELD___reliability = 8;
    static const int FIELD___units = 9;
    static const int FIELD___covIncrement = 10;
    static const int FIELD___presentValue = 11;
    static const int FIELD___presentPriority = 12;
    static const int FIELD___values = 13;
    static const int FIELD___valuesReality = 14;

    NANOCLR_NATIVE_DECLARE(SetValueWithPriority___VOID__R4__BOOLEAN__I4);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_BACnet;

#endif  //_ISMA_BACNET_NATIVE_H_
