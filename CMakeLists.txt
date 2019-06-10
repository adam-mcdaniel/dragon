#
# CMake file for building dragon
#
project(dragon)

# Minimum CMake doesnt really matter to me, I just picked mine
cmake_minimum_required(VERSION 3.10)
# We have to use C++17.
set(CMAKE_CXX_STANDARD 17)



# `dragon` will be our main executable
add_executable(dragon src/main.cpp)

# These are the headers/C++ files to use
set (components
"function"
"machine"
"option"
)


# Make a library for each C++ file
foreach (component IN LISTS components)
add_library (${component} src/${component}.cpp)
endforeach ()


# Link libraries
target_link_libraries(machine PRIVATE function option)
target_link_libraries(dragon PRIVATE machine)
