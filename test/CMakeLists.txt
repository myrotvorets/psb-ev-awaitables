if(ENABLE_MAINTAINER_MODE)
    string(REPLACE " " ";" COMPILE_OPTIONS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_MM} -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -Wno-disabled-macro-expansion")
    set_directory_properties(PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}")
    unset(COMPILE_OPTIONS)
endif()

set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src")

set(TEST_TARGET test_ev_awaitables)

add_executable(
    "${TEST_TARGET}"
    async_awaitable.cpp
    interval_awaitable.cpp
    io_awaitable.cpp
    signal_awaitable.cpp
    timed_io_awaitable.cpp
    timeout_awaitable.cpp
)

target_link_libraries("${TEST_TARGET}" PRIVATE ${PROJECT_NAME} GTest::gtest_main wwa::scope_action unofficial::libev::libev)
get_target_property(LIBEV_INCLUDE_DIR unofficial::libev::libev INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories("${TEST_TARGET}" SYSTEM PRIVATE "${LIBEV_INCLUDE_DIR}/libev")

set_target_properties(
    "${TEST_TARGET}"
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if(NOT CMAKE_CROSSCOMPILING)
    include(GoogleTest)
    gtest_discover_tests("${TEST_TARGET}")
endif()

if(ENABLE_COVERAGE)
    add_dependencies("${TEST_TARGET}" clean_coverage)
    add_dependencies(generate_coverage "${TEST_TARGET}")
endif()
