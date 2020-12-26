cmake_minimum_required(VERSION 3.5)
project(emulator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# Include directories
include_directories(${spi_emulator_SOURCE_DIR}/include)

add_library(emulator iterator.c array_iterator.c)