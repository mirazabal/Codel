#include "Codel.h"
#include "adapter.h"
#include <memory>
#include <string>
#include <string.h>
#include <vector>


#include <iostream>

//#if defined(__cplusplus)
//extern "C"
//{
//#endif 

void* mib_create_queue()
{
  return (void*) new mirazabal::Codel<std::string>();
}
  
void mib_delete_queue(void* queue)
{
	delete (mirazabal::Codel<std::string>*)(queue);
	queue = NULL;
}
  
void mib_enqueue(void* queue, const char* data, size_t size)
{
	((mirazabal::Codel<std::string>*)(queue))->enqueue(std::string(data,size));
};
  
char* mib_deque(void* queue, size_t* size)
{
  const std::string* str = ((mirazabal::Codel<std::string>*)(queue))->dequeue();
	if(str == nullptr){
		*size = 0;
		return NULL;
	}
  *size = str->size();
  return (char*)str->c_str(); 
}

void mib_pop(void* queue)
{
  ((mirazabal::Codel<std::string>*)(queue))->pop();
}

size_t mib_size_queue(void* queue)
{
  return ((mirazabal::Codel<std::string>*)(queue))->size();
}
//#if defined(__cplusplus)
//}
//#endif



