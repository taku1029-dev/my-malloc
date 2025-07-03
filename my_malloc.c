#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

int is_initialized = 0;
void* p_seg_start;
void* p_seg_end;

void malloc_init(){
  is_initialized = 1;
  p_seg_start = sbrk(0);
  p_seg_end = p_seg_start;
}

struct mcb{
  size_t size;
  int is_available;
};

void my_free(void** p_firstbyte){
  // p_firstbyte indecates the starting address of the memory block after mcb
  struct mcb* p_target_mcb = (struct mcb*) ((char*) p_firstbyte - sizeof(struct mcb));
  p_target_mcb->is_available = 1;
  p_firstbyte = NULL;
  return;
}

void* my_malloc(size_t size){
  if(!is_initialized){
    malloc_init();
  }

  // Changed p_current to char* to move around (2/7/2024)
  char* p_current = (char*) p_seg_start;
  struct mcb* p_current_mcb;
  int numOfBytes;

  while(p_current != p_seg_end){
    p_current_mcb = (struct mcb*) p_current;
    numOfBytes = p_current_mcb->size + sizeof(struct mcb);
    if(p_current_mcb->is_available == 1 && p_current_mcb->size >= size){
      p_current_mcb->is_available = 0;
      return p_current + sizeof(struct mcb);
    }
    p_current += numOfBytes;
  }

  // Request memory by moving sbrk
  numOfBytes = size + sizeof(struct mcb);
  sbrk(numOfBytes);
  p_seg_end = sbrk(0);
  struct mcb* p_new_mcb = (struct mcb*) p_current;
  p_new_mcb->size = size;
  p_new_mcb->is_available = 0;
  return p_seg_end - p_new_mcb->size;
}
