cmake_minimum_required(VERSION 3.10)
project(ALGO_sucesionFibonacci C)

set(CMAKE_C_STANDARD 99)

# Adicione seu executável ou biblioteca
add_executable(ALGO_sucesionFibonacci main.c)

# Linkar a biblioteca matemática
target_link_libraries(ALGO_sucesionFibonacci m)

