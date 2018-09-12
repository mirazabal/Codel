// Let Catch provide main():

//#define CATCH_CONFIG_MAIN

//#include "catch.hpp"
#include "../lib/adapter.h"
#include <pthread.h>

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <thread>
#include <chrono>

void queue_producer(void* q)
{
  const char data_enq[20] = "Mikel Irazabal";
	for(uint64_t i = 0; i < 10000; ++i){
  	mib_enqueue(q,data_enq,20);
		std::this_thread::sleep_for (std::chrono::milliseconds(1));
	}
}

void queue_consumer(void* q)
{
	uint64_t packetNotFound{0};
	std::this_thread::sleep_for (std::chrono::milliseconds(1));
	for(uint64_t i = 0; i < 10000; ++i){
		std::cout << "Value of i = " << i << std::endl;
		size_t size_of_data;
  	char* data_ = mib_deque(q, &size_of_data);
		if(data_ != NULL)
	  	mib_pop(q);
		else
			++packetNotFound;
		std::this_thread::sleep_for(std::chrono::microseconds(2000));
	}
		std::cout << "Packets lossed " << packetNotFound << std::endl;
}

int main()
{

  void* q = mib_create_queue();
	std::thread t_producer( [&]( ) { queue_producer(q); }  );
	std::thread t_consumer(  [&]( ) { queue_consumer(q); }  ); 


	t_producer.join();
	t_consumer.join();
	return 0;
}
/*
	printf("before mib create queue \n ");
  void* q = mib_create_queue();
  const char data_enq[200] = "Mikel Irazabal asd";
  printf("Before mib_enqueu \n");
  mib_enqueue(q,data_enq,200);
  printf("After mib_enqueu \n");
  const char data_enq2[100]  = "Bengoa Ugarte";
  mib_enqueue(q, data_enq2, 100);
  printf("After mib_enqueu second time \n");
  
  size_t size_of_data; // = (size_t*)malloc(sizeof(size_t));
  char* data_ = mib_deque(q, &size_of_data);
  mib_pop(q);
  printf("Printf data %s \n",data_);
  //free(data_);
  data_ = mib_deque(q, &size_of_data); 
  printf("Printf data second %s",data_);
  return 0;
}
*/

/*

TEST_CASE( "Codel constructor", "[single-file ]") {
  
  void* q = mib_create_queue();
  const char data_enq[200] = "First data";
  mib_enqueue(q,data_enq,200);
  const char data_enq2[100]  = "Second data";
  mib_enqueue(q, data_enq2, 100);
  
  size_t size_of_data; 
  char* data_ = mib_deque(q, &size_of_data);
  mib_pop(q);
  REQUIRE( size_of_data == 200 );
  REQUIRE( std::string(data_,200) == std::string(data_enq,200) );
  
  data_ = mib_deque(q, &size_of_data); 
  REQUIRE( size_of_data == 100 );
  REQUIRE( std::string(data_,100) == std::string( data_enq2,100) );
  mib_pop();
}


int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}



int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
*/
/*
int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
*/

/*
TEST_CASE( "Codel constructor and memory footprint", "[single-file ]") {
    auto val1 = getValue();   
    void* retVal =  mib_create_queue();
    REQUIRE( retVal != NULL );
    void* p = malloc(1024*1024);
    auto val2 = getValue();
    REQUIRE( val1 != val2 );
    free(p);
    mib_delete_queue(retVal);
    auto val3 = getValue();   
    REQUIRE( val1 == val3 );
}


TEST_CASE( "Codel start queuing packets, no delay", "[single-file ]") {
    auto val1 = getValue();   
    void* retVal =  mib_create_queue();
    REQUIRE( retVal != NULL );
    char* packet = new char[1024];
    mib_enqueue(retVal, packet, 1024);
    char* data;
    size_t size;

    mib_deque(retVal,data,size);
    REQUIRE( data == packet );
    REQUIRE( size == 1024 );
    void* p = malloc(1024*1024);
    auto val2 = getValue();
    REQUIRE( val1 != val2 );
    free(p);
    mib_delete_queue(retVal);
    auto val3 = getValue();   
    REQUIRE( val1 == val3 );
}
*/

