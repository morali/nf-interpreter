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

    static const int FIELD___aoutputId = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(AOSetVoltage___VOID__U2);
    NANOCLR_NATIVE_DECLARE(AOSetDuty___VOID__U4);
    NANOCLR_NATIVE_DECLARE(AOSetFreq___VOID__U4);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_DI
{
    static const int FIELD_STATIC__Empty = 1;

    static const int FIELD___dinputId = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(DIGetNative___BOOLEAN);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_DO
{
    static const int FIELD_STATIC__Empty = 2;

    static const int FIELD___doutputId = 1;
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
    NANOCLR_NATIVE_DECLARE(GetAONative___STATIC__SZARRAY_iSMALocalIOAO);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_UI
{
    static const int FIELD_STATIC__Empty = 3;

    static const int FIELD___id = 1;
    static const int FIELD___name = 2;
    static const int FIELD___config = 3;

    NANOCLR_NATIVE_DECLARE(SetConfigNative___VOID__iSMALocalIOUIConfiguration);
    NANOCLR_NATIVE_DECLARE(GetVoltageNative___I4);
    NANOCLR_NATIVE_DECLARE(GetResistanceNative___I4);
    NANOCLR_NATIVE_DECLARE(GetDigitalNative___BOOLEAN);

    //--//

};

struct Library_isma_localio_native_iSMA_LocalIO_UIConfiguration
{
    static const int FIELD__resolution = 1;
    static const int FIELD__gain = 2;
    static const int FIELD__measureVoltage = 3;
    static const int FIELD__measureResistance = 4;
    static const int FIELD__filterTime = 5;


    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_LocalIO;

#endif  //_ISMA_LOCALIO_NATIVE_H_
