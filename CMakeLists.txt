cmake_minimum_required(VERSION 3.21)
project(DContainers)

set(CMAKE_CXX_STANDARD 20)

add_executable(DContainers
        src/DVector.hpp
        src/DArray.hpp
        src/Span/Span.hpp
        src/Span/DSpan.hpp
        src/Span/SpanWrapper.hpp
        )

# TESTING

# gtest dependency
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)

# gtest: For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        unit_tests
        src/DVector.hpp
        src/DArray.hpp
        tests/DVector_tests.cpp
        tests/DArray_tests.cpp)
target_link_libraries(
        unit_tests
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
