cmake_minimum_required(VERSION 3.0)
project(ft)

add_executable(ft.tests
    tests/ft.tests.cpp
    tests/ft.test_helpers.cpp
    ft.buffer.c
    ft.c
    ft.crc.c
    ft.convert.c
    ft.parser.c
)
find_package(GTest REQUIRED)
target_link_libraries(ft.tests PRIVATE GTest::GTest GTest::Main)
target_compile_options(ft.tests PRIVATE -std=c++11)