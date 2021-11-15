cmake_minimum_required(VERSION 3.20)

project(MatrixClass)

aux_source_directory(. srcs)

add_executable(matmul.out ${srcs})