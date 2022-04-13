find_package(GTest QUIET)

if(NOT GTEST_FOUND)
    # Download googletest
    configure_file(${CMAKE_SOURCE_DIR}/cmake/GoogleTest-CMakeLists.txt.in
            ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Defines gtest and gtest_main targets.
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
            ${CMAKE_BINARY_DIR}/googletest-build
            EXCLUDE_FROM_ALL)
endif()

# Now simply link against gtest
add_executable(DContainers_test
        unit/DArray_tests.cpp
        unit/DVector_tests.cpp
        unit/Span/Span_tests.cpp
        unit/Span/DSpan_tests.cpp
        unit/Span/SpanWrapper_tests.cpp
        )

target_compile_features(DContainers_test PRIVATE cxx_std_20)
target_link_libraries(DContainers_test gtest_main DContainers::DContainers)

add_test(NAME DContainers_test
        COMMAND DContainers_test)
