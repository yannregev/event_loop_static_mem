include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0
)
FetchContent_MakeAvailable(unity)

function(add_unit_test)
   cmake_parse_arguments(ARG
        ""            # no boolean flags
        "NAME"        # single-value
        "SOURCES"     # multi-value
        ${ARGN}
    )
    add_executable(${ARG_NAME} ${ARG_SOURCES})
    target_link_libraries(${ARG_NAME} PRIVATE libev unity)
    add_test(NAME ${ARG_NAME} COMMAND ${ARG_NAME})
endfunction()


add_unit_test(
    NAME test_queue
    SOURCES test_queue.c)

set (TEST_EVENTQUEUE_SOURCES
    test_eventqueue.c
    fakes/fake_queue.c
    ${CMAKE_SOURCE_DIR}/src/eventqueue.c
)
add_unit_test(
    NAME test_eventqueue
    SOURCES ${TEST_EVENTQUEUE_SOURCES})

set (TEST_PERIODICFUNCTIONS_SOURCES
    test_periodicfunctions.c
    fakes/fake_eventqueue.c
    ${CMAKE_SOURCE_DIR}/src/periodicfunctions.c
)
add_unit_test(
    NAME test_periodicfunctions
    SOURCES ${TEST_PERIODICFUNCTIONS_SOURCES})

set (TEST_DELAYEDFUNCTIONS_SOURCES
    test_delayedfunctions.c
    fakes/fake_eventqueue.c
    ${CMAKE_SOURCE_DIR}/src/delayedfunctions.c
)
add_unit_test(
    NAME test_delayedfunctions
    SOURCES ${TEST_DELAYEDFUNCTIONS_SOURCES})