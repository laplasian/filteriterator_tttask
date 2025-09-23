cmake_minimum_required(VERSION 3.10)
project(filteriterator CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(filteriterator_tests_sfinae tests.cpp)
target_link_libraries(filteriterator_tests_sfinae GTest::gtest_main)

add_executable(filteriterator_tests_concepts tests.cpp)
target_compile_definitions(filteriterator_tests_concepts PRIVATE USE_CONCEPTS)
target_link_libraries(filteriterator_tests_concepts GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(filteriterator_tests_sfinae)
gtest_discover_tests(filteriterator_tests_concepts)