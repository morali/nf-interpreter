//
// Copyright (c) 2019 The nanoFramework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>
#include "board.h"

void HeapLocation(unsigned char*& baseAddress, unsigned int& sizeInBytes)
{
    NATIVE_PROFILE_PAL_HEAP();
    baseAddress = (unsigned char*)FlexSPI2_AMBA_BASE;   //start of Hyper RAM
    sizeInBytes = 0x800000;                             //8MB
}
