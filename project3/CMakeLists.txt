cmake_minimum_required(VERSION 3.20)

project(MatrixMultiplication)

aux_source_directory(. srcs)

add_executable(matmul ${srcs})