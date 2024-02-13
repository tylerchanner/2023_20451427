# calc_cmake/CMakeLists.txt
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# This project contains a dynamic library
add_library(maths SHARED adder.cpp)
# Note: We remove the STATIC keyword to build a dynamic library by default

# It also contains an executable
add_executable(calc calc.cpp)

# The executable uses the library
target_link_libraries(calc maths)

# Define the preprocessor flag 'maths_EXPORTS' when building the library
target_compile_definitions(maths PRIVATE maths_EXPORTS)

# /calc_cmake/CMakeLists.txt
