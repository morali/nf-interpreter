
#include "bacnet_objects.h"

static BACnetObj_t *av_head;
static BACnetObj_t *bv_head;

/**
 * @brief  Initialize head pointer of BACnetObj_t structure.
 *         It's a linked list which hold various bacnet objects.
 * @note
 * @retval Returns pointer to linked list structer if successful, else NULL pointer.
 */
BACnetObj_t *B_Initialize(BACnetObj_t *head) {

  head = malloc(sizeof(BACnetObj_t));
  if (head == NULL)
    return NULL;
  head->object = malloc(sizeof(BACnetAV_t));
  if (head->object == NULL) {
    free(head);
    return NULL;
  }
  return head;
}

BACnetObj_t *B_RetHead(BACtype_t type) {
  switch (type) {
  case 0:
    return av_head;
  case 1:
    return bv_head;
  default:
    return NULL;
  }
}

void B_AddObj(BACnetObj_t *head, void * obj) {
  while(head->next != NULL) {
    head = head->next;
  }

  current->next = malloc(List_t);

}

uint8_t AddObject(BACnetObj_t *head) {
  uint8_t error = 1;

  switch (type) {
  case 0: {
    BACnetAV_t *current = retObj(av);

    if (current == NULL) {
      current = malloc(sizeof(BACnetAV_t));
      if (current == NULL)
        break;
      // memset(current, 0, sizeof(BACnetAV_t));
      error = 0;
      break;
    } else {
      while (current->next != NULL) {
        current = current->next;
      }
      current->next = malloc(sizeof(BACnetAV_t));
      if (current->next == NULL)
        break;
      memset(current->next, 0, sizeof(BACnetAV_t));
      error = 0;
      break;
    }
  }
  default:
    error = 1;
  }
  return error;
}

uint32_t Size(BACtype type, void *bacptr) {
  uint32_t size = 0;
  switch (type) {
  case 0: {
    BACnetAV_t *current = (BACnetAV_t *)bacptr;
    while (current != NULL) {
      size++;
      current = current->next;
    }
    break;
  }
  case 1: {
    BACnetBV_t *current = (BACnetBV_t *)bacptr;
    while (current != NULL) {
      size++;
      current = current->next;
    }
    break;
  }
  default:
    bacptr = NULL;
    break;
  }
  return size;
}

uint32_t FindId(BACtype type, uint32_t object_instance) {
  uint32_t i = 0;
  switch (type) {
  case 0: {
    BACnetAV_t *current = retObj(av);
    while (current != NULL) {
      if (current->id == object_instance)
        return i;
      current = current->next;
      i++;
    }
    break;
  }
  default:
    return 0;
  }
  return 0;
}

void *FindIdRet(BACtype type, uint32_t object_instance) {
  switch (type) {
  case 0: {
    BACnetAV_t *current = retObj(av);
    while (current != NULL) {
      if (current->id == object_instance)
        return current;
      current = current->next;
    }
    break;
  }
  default:
    return NULL;
  }
  return NULL;
}

void LinkAtFront() {}