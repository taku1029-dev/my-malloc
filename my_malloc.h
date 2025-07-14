#ifndef MY_MALLOC
#define MY_MALLOC

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct mcb{
  size_t size;
  int is_available;
  struct mcb* next;
} mcb_t;

extern int is_initialized;
extern mcb_t* head;

void malloc_init(size_t size);
void my_free(void* p_firstbyte);
void* my_malloc(size_t size);

#ifdef __cplusplus
}
#endif

#endif