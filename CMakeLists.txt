cmake_minimum_required (VERSION 2.8)
project (main)

enable_language(CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set_target_properties(Node PROPERTIES LINKER_LANGUAGE CXX)

include_directories(/home/oai/workspace_2/Codel/src/)
include_directories(${main_SOURCE_DIR})
include_directories(${main_SOURCE_DIR}/src)
include_directories(${main_SOURCE_DIR}/../src)


file(GLOB main_SRC
    "*.h"
    "*.cpp"
    "src/*.cpp"
		"lib/*.h"
)

add_executable(main ${main_SRC} )

find_library(CODEL_LIB mib_codel HINTS /home/oai/workspace_2/Codel/lib)

TARGET_LINK_LIBRARIES(main PUBLIC ${CODEL_LIB} pthread )

