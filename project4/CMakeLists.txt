cmake_minimum_required(VERSION 3.20)

project(MatrixClass)

SET(CMAKE_BUILD_TYPE "Release")

aux_source_directory(. srcs)

add_executable(matmul.out ${srcs})