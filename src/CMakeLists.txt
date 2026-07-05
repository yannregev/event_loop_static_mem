cmake_minimum_required(VERSION 3.10.0)

add_library(libev STATIC 
    eventqueue.c
    delayedfunctions.c
    periodicfunctions.c
    includes.c
    queue.c
)

target_include_directories(libev PUBLIC ${CMAKE_SOURCE_DIR}/include)
