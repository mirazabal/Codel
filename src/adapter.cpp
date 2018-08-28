#include "Codel.h"
#include "adapter.h"
#include <memory>
#include <string>
#include <vector>

void* mib_create_queue(){
  return (void*) new mirazabal::Codel<std::string>();
}
  
void mib_delete_queue(void* queue){
    delete (mirazabal::Codel<std::string>*)(queue);
  }
  
void mib_enqueue(void* queue, const char* data, size_t size){
  ((mirazabal::Codel<std::string>*)(queue))->enqueue(std::string(data,size));
  };
  
void mib_deque(void* queue, char* data, size_t* size)
{
  const std::string* val = ((mirazabal::Codel<std::string>*)(queue))->dequeue();
  data = (char*)val->data();
  *size = val->size(); 
}

void mib_pop(void* queue){
  ((mirazabal::Codel<std::string>*)(queue))->pop();
}


