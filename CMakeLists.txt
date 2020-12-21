cmake_minimum_required(VERSION 3.5)
project(spi_emulator C)
enable_testing()

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)
add_subdirectory(unit_test)