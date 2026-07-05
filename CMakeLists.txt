cmake_minimum_required(VERSION 3.10.0)
project(libev VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 23)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set (SOURCES  
    eventqueue.c
    delayedfunctions.c
    periodicfunctions.c
    includes.c
    queue.c
)

set(HEADERS
    includes.h
    eventqueue.h
    delayedfunctions.h
    periodicfunctions.h
    queue.h
)

add_library(event_loop_static_mem STATIC 
    ${SOURCES}
    ${HEADERS}
)

#target_compile_options(libev PUBLIC -Wall)