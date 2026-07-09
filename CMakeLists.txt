cmake_minimum_required(VERSION 3.14)
project(libev VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 23)

option(BUILD_TESTS "Build tests" ON)

set(LIBENV_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

#target_compile_options(libev PUBLIC -Wall)