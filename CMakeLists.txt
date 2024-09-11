#cmake_minimum_required(VERSION 3.0) # I would advise to update

cmake_minimum_required(VERSION 3.24.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(algae)
#include_directories(sources)
#target_include_directories(algae PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
file(GLOB sources src/*.cpp) # upper case variable names reserved for exported variables by convention
file(GLOB headers src/*.h) # upper case variable names reserved for exported variables by convention
add_library(algae ${sources} ${headers}) # STATIC by default

