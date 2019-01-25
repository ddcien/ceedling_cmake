cmake_minimum_required(VERSION 3.6)
project(cmock)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmock/cmock.cmake)
add_subdirectory(examples)

