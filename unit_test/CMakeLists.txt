cmake_minimum_required(VERSION 3.5)
project(unit_test CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

set(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CPPUTEST_DIR ${CURRENT_DIR}/lib/cpputest)
set(MAIN_DIR ${spi_emulator_SOURCE_DIR})

# Temporary disable leak detector. Uncomment the line below to undo
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CPPUTEST_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h")

# Include directories
include_directories(${MAIN_DIR}/include)
include_directories(${MAIN_DIR}/lib/ini/include)
include_directories(${CPPUTEST_DIR}/include)

# Library directories
link_directories(${CPPUTEST_DIR})
link_directories(${MAIN_DIR}/lib/ini)

#######################################
############# Executables #############
#######################################

# Iterator
add_executable(IteratorTests AllTests.cpp IteratorTests.cpp Fakes.cpp)
target_link_libraries(IteratorTests emulator CppUTest CppUTestExt)

# ArrayIterator
add_executable(ArrayIteratorTests AllTests.cpp ArrayIteratorTests.cpp)
target_link_libraries(ArrayIteratorTests emulator CppUTest CppUTestExt)

add_test(NAME IteratorTests COMMAND IteratorTests -v)
add_test(NAME ArrayIteratorTests COMMAND ArrayIteratorTests -v)
