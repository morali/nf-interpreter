#include "isma_localio_native.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_localio_native_iSMA_LocalIO_AO::AOInit___VOID,
    Library_isma_localio_native_iSMA_LocalIO_AO::AOSetVoltage___VOID__BOOLEAN,
    Library_isma_localio_native_iSMA_LocalIO_AO::AOGetVoltage___BOOLEAN,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_localio_native_iSMA_LocalIO_DI::DIGetNative___BOOLEAN,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_localio_native_iSMA_LocalIO_DO::DOSetNative___VOID__BOOLEAN,
    Library_isma_localio_native_iSMA_LocalIO_DO::DOGetNative___BOOLEAN,
    Library_isma_localio_native_iSMA_LocalIO_DO::DOInitNative___BOOLEAN,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDIsNative___STATIC__SZARRAY_iSMALocalIODI,
    Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetDOsNative___STATIC__SZARRAY_iSMALocalIODO,
    Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetUIsNative___STATIC__SZARRAY_iSMALocalIOUI,
    Library_isma_localio_native_iSMA_LocalIO_LocalIOController::GetAONative___STATIC__SZARRAY_iSMALocalIOAO,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_localio_native_iSMA_LocalIO_UI::UIInit___VOID,
    Library_isma_localio_native_iSMA_LocalIO_UI::UIConfigNative___VOID,
    Library_isma_localio_native_iSMA_LocalIO_UI::UIGetNative___BOOLEAN,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_LocalIO =
{
    "iSMA.LocalIO", 
    0xD3237ED9,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    { 100, 0, 0, 0 }
};
