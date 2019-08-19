#ifndef _ISMA_GPIO_NATIVE_H_
#define _ISMA_GPIO_NATIVE_H_

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>

struct Library_isma_gpio_native_iSMA_GPIO_DIPSwitch
{
    static const int FIELD_STATIC__Empty = 0;

    static const int FIELD___switchId = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(DIPSwitchGetNative___U1);

    //--//

};

struct Library_isma_gpio_native_iSMA_GPIO_DIPSwitchController
{
    NANOCLR_NATIVE_DECLARE(GetDIPSwitchesNative___STATIC__SZARRAY_iSMAGPIODIPSwitch);

    //--//

};

struct Library_isma_gpio_native_iSMA_GPIO_LED
{
    static const int FIELD_STATIC__Empty = 1;

    static const int FIELD___ledId = 1;
    static const int FIELD___name = 2;

    NANOCLR_NATIVE_DECLARE(LEDSetNative___VOID__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(LEDGetNative___BOOLEAN);

    //--//

};

struct Library_isma_gpio_native_iSMA_GPIO_LEDController
{
    NANOCLR_NATIVE_DECLARE(GetLedsNative___STATIC__SZARRAY_iSMAGPIOLED);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_iSMA_GPIO;


#endif  //_ISMA_GPIO_NATIVE_H_
