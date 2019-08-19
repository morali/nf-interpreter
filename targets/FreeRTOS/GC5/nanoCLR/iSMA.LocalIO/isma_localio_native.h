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
#ifndef _ISMA_LOCALIO_NATIVE_H_
#define _ISMA_LOCALIO_NATIVE_H_

struct Library_isma_localio_native_iSMA_LocalIO_AO
{
    static const int FIELD_STATIC__Empty = 0;

    static const int FIELD___aoutputId = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(AOInit___VOID);
    NANOCLR_NATIVE_DECLARE(AOSetVoltage___VOID__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(AOGetVoltage___BOOLEAN);

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

    static const int FIELD___uinputId = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(UIInit___VOID);
    NANOCLR_NATIVE_DECLARE(UIConfigNative___VOID);
    NANOCLR_NATIVE_DECLARE(UIGetNative___BOOLEAN);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_LocalIO;

#endif  //_ISMA_LOCALIO_NATIVE_H_
