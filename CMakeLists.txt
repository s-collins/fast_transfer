cmake_minimum_required(VERSION 3.0)
project(fast_transfer)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

add_subdirectory(fast_transfer)
add_subdirectory(tests)
