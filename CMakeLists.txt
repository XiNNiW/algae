cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)

project(algae)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )

set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

#GTEST
# set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/googletest)
# add_subdirectory(${GTEST_ROOT})
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

#Algae (DSP)
add_library( algae STATIC
             ${PROJECT_SOURCE_DIR}/dsp.cpp)

#Tests
add_executable( AlgaeTests 
    ${TEST_DIR}/math_functions_test.cpp
    ${TEST_DIR}/oscillator_test.cpp
    ${TEST_DIR}/filter_test.cpp
    ${TEST_DIR}/control_test.cpp
)
target_link_libraries(AlgaeTests algae)
target_link_libraries(AlgaeTests GTest::GTest GTest::Main)
gtest_discover_tests(AlgaeTests)
