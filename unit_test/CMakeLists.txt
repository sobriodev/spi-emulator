cmake_minimum_required(VERSION 3.5)
project(unit_test CXX)

SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(CPPUTEST_DIR ${CURRENT_DIR}/lib/cpputest)
SET(MAIN_DIR ${spi_emulator_SOURCE_DIR})

# Enable memory leaks detection for cpputest
SET(CMAKE_CXX_FLAGS "-include ${CPPUTEST_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h")

# Include directories
include_directories(${MAIN_DIR}/include)
include_directories(${MAIN_DIR}/lib/ini/include)
include_directories(${CPPUTEST_DIR}/include)

# Library directories
link_directories(${CPPUTEST_DIR})
link_directories(${MAIN_DIR}/lib/ini)

add_executable(TestDummy AllTests.cpp TestDummy.cpp)
target_link_libraries(TestDummy emulator CppUTest CppUTestExt)

add_test(NAME TestDummy COMMAND TestDummy -v)
