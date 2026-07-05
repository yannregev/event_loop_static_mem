include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0
)
FetchContent_MakeAvailable(unity)

function(add_unit_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE libev unity)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_unit_test(test_queue test_queue.c)
add_unit_test(test_eventqueue test_eventqueue.c)