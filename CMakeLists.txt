#cmake_minimum_required(VERSION 3.0) # I would advise to update

cmake_minimum_required(VERSION 3.24.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(algae)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wsign-compare -Wsign-conversion -Wunreachable-code -Wcast-align -Wno-implicit-fallthrough -Wno-maybe-uninitialized -Wno-ignored-qualifiers -Wswitch-enum -Wredundant-decls -Wno-strict-overflow -Wshadow -Wfloat-equal -Wmissing-field-initializers -Woverloaded-virtual -Wreorder -Wzero-as-null-pointer-constant")
endif()
#include_directories(sources)
#target_include_directories(algae PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
file(GLOB sources src/*.cpp) # upper case variable names reserved for exported variables by convention
file(GLOB headers src/*.h) # upper case variable names reserved for exported variables by convention
add_library(algae STATIC ${sources} ${headers}) # STATIC by default
target_compile_features(algae PUBLIC cxx_std_17)
target_include_directories(algae PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

