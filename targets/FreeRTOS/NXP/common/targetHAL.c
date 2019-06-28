//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "stdint.h"
#include "stdbool.h"
#include "cmsis_gcc.h"
#include <nanoWeak.h>
<<<<<<< HEAD
=======
#include <platform_target_capabilities.h>
>>>>>>> 9cb2323b44b74e57dca1f46f1b2a8875c10c49b6

void HAL_AssertEx()
{
    __BKPT(0);
    while(true) { /*nop*/ }
}

#if !defined(BUILD_RTM)

void HARD_Breakpoint() 
{
    __BKPT(0);
    while(true) { /*nop*/ }
};

#endif  // !defined(BUILD_RTM)

// Provides information whether the configuration block storage requires erase command before sending the update command
// The 'weak' implementation for NXP targets is true
// If a target implements the store differently it has to provide a 'strong' implementation of this.
__nfweak bool Target_ConfigUpdateRequiresErase()
{
    return true;
};

bool Target_HasNanoBooter()
{ 
    return true; 
};
<<<<<<< HEAD
=======

// declarations of platform capabilities
uint32_t GetPlatformCapabilities()
{ 
    return 0; 
};

// declarations of target capabilities
// If a target has something to declare it has to provide a 'strong' implementation of this.
__nfweak uint32_t GetTargetCapabilities()
{ 
    return TargetCapabilities_JtagUpdate; 
};
>>>>>>> 9cb2323b44b74e57dca1f46f1b2a8875c10c49b6
