cmake_minimum_required(VERSION 3.0) # I would advise to update

project(algae)
include_directories(include)
file(GLOB sources src/*.cpp) # upper case variable names reserved for exported variables by convention
add_library(algae ${sources}) # STATIC by default
target_include_directories(algae PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
