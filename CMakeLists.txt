cmake_minimum_required(VERSION 3.22)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Mandelbrot main.cpp)

target_link_libraries(Mandelbrot sfml-graphics sfml-audio)
