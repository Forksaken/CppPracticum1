cmake_minimum_required(VERSION 3.19)
project(pi_calc)

set(CMAKE_CXX_STANDARD 23)

# LongNumber library
add_library(LongNumber src/LongNumber.cpp)

# executable for testing
add_executable(TestLongNumber tests/tests.cpp)
target_link_libraries(TestLongNumber LongNumber)

# executable for calculating Pi
add_executable(CalculatePi src/PiCalculator.cpp)
target_link_libraries(CalculatePi LongNumber)

add_executable(pi_calc src/main.cpp
        src/LongNumber.cpp
        src/LongNumber.h)
