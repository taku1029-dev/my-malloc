#include "my_malloc.h"

int is_initialized = 0;
mcb_t* head;

void malloc_init(size_t size){
  is_initialized = 1;
  head = (mcb_t*)sbrk(size + sizeof(mcb_t));
  head->size = size;
  head->is_available = 1;
  head->next = NULL;
}

void my_free(void* p_firstbyte){
  if(p_firstbyte == NULL){
    return;
  }
  
  // p_firstbyte indecates the starting address of the memory block after mcb
  mcb_t* p_target_mcb = (mcb_t*) ((char*) p_firstbyte - sizeof(mcb_t));
  p_target_mcb->is_available = 1;
}

void* my_malloc(size_t size){
  if(!is_initialized){
    malloc_init(size);
  }

  mcb_t* p_current_mcb = head;
  mcb_t* prev = NULL;
  int numOfBytes;

  while(p_current_mcb != NULL){
    numOfBytes = p_current_mcb->size + sizeof(mcb_t);
    if(p_current_mcb->is_available == 1 && p_current_mcb->size >= size){
      p_current_mcb->is_available = 0;
      return (char*)p_current_mcb + sizeof(mcb_t);
    }
    prev = p_current_mcb;
    p_current_mcb = p_current_mcb->next;
  }

  // Request memory by moving sbrk
  numOfBytes = size + sizeof(mcb_t);
  mcb_t* p_new_mcb = (mcb_t*)sbrk(numOfBytes);
  prev->next = p_new_mcb;
  p_new_mcb->size = size;
  p_new_mcb->is_available = 0;
  p_new_mcb->next = NULL;
  return (char*)p_new_mcb + sizeof(mcb_t);
}