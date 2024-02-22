cmake_minimum_required(VERSION 3.19)
project(pi_calc)

set(CMAKE_CXX_STANDARD 23)

# LongNumber library
add_library(LongNumber src/LongNumber/LongNumber.cpp)
add_library(PiCalculate STATIC src/PiCalculator/PiCalculator.cpp)
target_include_directories(PiCalculate PUBLIC src/PiCalculator)
target_link_libraries(PiCalculate LongNumber)
# executable for testing
add_executable(TestLongNumber tests/tests.cpp)
target_link_libraries(TestLongNumber LongNumber)

# executable for calculating Pi
add_executable(pi_calc src/main.cpp)
target_link_libraries(pi_calc PiCalculate)
include_directories(src/PiCalculator)