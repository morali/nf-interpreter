

#include "isma_bacnet_bacdcode.h"

/**
 * @brief  Reimplementation of function from bacdcode.c, which works with iSMA BACnet
 * @note   
 * @param  *apdu: 
 * @param  *char_string: 
 * @retval 
 */
int encode_application_character_string_isma(uint8_t *apdu, const char* string) {
  int len = 0;
  int string_len = 0;
    
  BACNET_CHARACTER_STRING bacnetString;
  characterstring_init_ansi(&bacnetString, string);

  string_len = (int)characterstring_length(&bacnetString) + 1 /* for encoding */;
  len = encode_tag(&apdu[0], BACNET_APPLICATION_TAG_CHARACTER_STRING, false, (uint32_t)string_len);
  if ((len + string_len) < MAX_APDU) {
    len += encode_bacnet_character_string(&apdu[len], &bacnetString);
  } else {
    len = 0;
  }

  return len;
}