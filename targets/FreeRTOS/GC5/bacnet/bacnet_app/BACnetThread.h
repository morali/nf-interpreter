#ifndef BACNETTHREAD_H_
#define BACNETTHREAD_H_

#define MAX_BACNET_TASK 512
#define MAX_VALUE_LEN 64

#undef TEST

/* Define u32_t type used in BACnet files as uint32_t */
#define u32_t uint32_t

void vBACnetThread(void *parameters);

#endif /* BACNETTHREAD_H_ */