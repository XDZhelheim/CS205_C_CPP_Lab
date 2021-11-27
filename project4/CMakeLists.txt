cmake_minimum_required(VERSION 3.10)

project(MatrixClass)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. srcs)

add_executable(matmul.out ${srcs})