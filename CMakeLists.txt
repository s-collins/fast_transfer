cmake_minimum_required(VERSION 3.0)
project(ft)

add_executable(ft_tests
    ft_tests.cpp
    ft.buffer.c
    ft.c
    ft.crc.c
    ft.parser.c
    Convert.c
)
find_package(GTest REQUIRED)
target_link_libraries(ft_tests PRIVATE GTest::GTest GTest::Main)
target_compile_options(ft_tests PRIVATE -std=c++11)