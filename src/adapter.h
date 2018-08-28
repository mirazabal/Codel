#ifndef ADAPTER_CODEL_H
#define ADAPTER_CODEL_H

#if defined(__cplusplus)
extern "C"
{
#endif 

#include <stddef.h>

//  __attribute__(visibility("default"))
  void* mib_create_queue();
  
//  __attribute__(visibility("default"))
  void mib_delete_queue(void*);
  
//  __attribute__(visibility("default"))
void mib_enqueue(void* queue, const char* data, size_t size){
  
//  __attribute__(visibility("default"))
  void mib_deque_2(void* queue, char* data, size_t* size);
 
//  __attribute__(visibility("default"))
  void mib_pop(void* queue);

#if defined(__cplusplus)
}
#endif

#endif /* ADAPTER_CODEL_H */

