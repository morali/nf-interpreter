#include "isma_gpio_native.h"

static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_gpio_native_iSMA_GPIO_DIPSwitch::DIPSwitchGetNative___U1,
    NULL,
    NULL,
    Library_isma_gpio_native_iSMA_GPIO_DIPSwitchController::GetDIPSwitchesNative___STATIC__SZARRAY_iSMAGPIODIPSwitch,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_isma_gpio_native_iSMA_GPIO_LED::LEDSetNative___VOID__BOOLEAN,
    Library_isma_gpio_native_iSMA_GPIO_LED::LEDGetNative___BOOLEAN,
    NULL,
    NULL,
    Library_isma_gpio_native_iSMA_GPIO_LEDController::GetLedsNative___STATIC__SZARRAY_iSMAGPIOLED,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_GPIO =
{
    "iSMA.GPIO", 
    0x6BB2F99A,
    method_lookup,
    ////////////////////////////////////////////////////////////////////////////////////
    // check if the version bellow matches the one in AssemblyNativeVersion attribute //
    ////////////////////////////////////////////////////////////////////////////////////
    { 100, 0, 0, 0 }
};
