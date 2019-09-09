/*
 * Created on Fri Aug 23 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#ifndef _ISMA_LOCALIO_NATIVE_H_
#define _ISMA_LOCALIO_NATIVE_H_

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>

struct Library_isma_localio_native_iSMA_LocalIO_AO
{
    static const int FIELD_STATIC__Empty = 0;

    static const int FIELD___id = 1;
    static const int FIELD___name = 2;
    static const int FIELD___mode = 3;
    static const int FIELD___frequency = 4;

    NANOCLR_NATIVE_DECLARE(SetConfigNative___VOID);
    NANOCLR_NATIVE_DECLARE(SetDutyNative___VOID__U1);
    NANOCLR_NATIVE_DECLARE(SetVoltageNative___VOID__U2);
    NANOCLR_NATIVE_DECLARE(SetDigitalNative___VOID__BOOLEAN);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_DI
{
    static const int FIELD_STATIC__Empty = 1;

    static const int FIELD___id = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(DIGetNative___BOOLEAN);
    NANOCLR_NATIVE_DECLARE(DIGetCounterNative___U4);
    NANOCLR_NATIVE_DECLARE(DISetCounterNative___VOID__U4);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_DO
{
    static const int FIELD_STATIC__Empty = 2;

    static const int FIELD___id = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(DOSetNative___VOID__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(DOGetNative___BOOLEAN);
    NANOCLR_NATIVE_DECLARE(DOToggleNative___VOID);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_LocalIOController
{
    NANOCLR_NATIVE_DECLARE(GetDIsNative___STATIC__SZARRAY_iSMALocalIODI);
    NANOCLR_NATIVE_DECLARE(GetDOsNative___STATIC__SZARRAY_iSMALocalIODO);
    NANOCLR_NATIVE_DECLARE(GetUIsNative___STATIC__SZARRAY_iSMALocalIOUI);
    NANOCLR_NATIVE_DECLARE(GetAOsNative___STATIC__SZARRAY_iSMALocalIOAO);
    NANOCLR_NATIVE_DECLARE(GetTOsNative___STATIC__SZARRAY_iSMALocalIOTO);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_TO
{
    static const int FIELD_STATIC__Empty = 3;

    static const int FIELD___id = 1;
    static const int FIELD___name = 2;
    static const int FIELD___mode = 3;
    static const int FIELD___frequency = 4;

    NANOCLR_NATIVE_DECLARE(SetConfigNative___VOID);
    NANOCLR_NATIVE_DECLARE(SetDutyNative___VOID__U1);
    NANOCLR_NATIVE_DECLARE(SetDigitalNative___VOID__BOOLEAN);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_UI
{
    static const int FIELD_STATIC__Empty = 4;

    static const int FIELD___id = 1;
    static const int FIELD___name = 2;
    static const int FIELD___resolution = 3;
    static const int FIELD___gain = 4;
    static const int FIELD___filterTime = 5;
    static const int FIELD___measureVoltage = 6;
    static const int FIELD___measureResistance = 7;

    NANOCLR_NATIVE_DECLARE(SetConfigNative___VOID);
    NANOCLR_NATIVE_DECLARE(GetVoltageNative___I2);
    NANOCLR_NATIVE_DECLARE(GetResistanceNative___U4);
    NANOCLR_NATIVE_DECLARE(GetDigitalNative___BOOLEAN);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_LocalIO;

#endif  //_ISMA_LOCALIO_NATIVE_H_
