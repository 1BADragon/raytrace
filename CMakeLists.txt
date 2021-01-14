cmake_minimum_required(VERSION 3.16)
project(raytrace)


set(srcs
    src/main.cpp)

add_executable(${PROJECT_NAME} ${srcs})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
