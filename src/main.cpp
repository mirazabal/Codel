// Let Catch provide main():

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "adapter.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"



int Factorial( int number ) {
   return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
// return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE( "Codel constructor", "[single-file ]") {
    void* retVal =  mib_create_queue();
    REQUIRE(  retVal != NULL );
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




