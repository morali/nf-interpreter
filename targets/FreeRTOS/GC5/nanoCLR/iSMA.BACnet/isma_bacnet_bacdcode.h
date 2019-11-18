
#ifndef _ISMA_BACNET_BACDCODE_H_
#define _ISMA_BACNET_BACDCODE_H_

#include <string.h>

#include "bacdcode.h"
#include "bacdef.h"
#include "bacenum.h"
#include "bacint.h"
#include "bacreal.h"
#include "bacstr.h"
#include "bits.h"

int encode_application_character_string_isma(uint8_t *apdu, const char* string);


#endif //_ISMA_BACNET_BACDCODE_H_