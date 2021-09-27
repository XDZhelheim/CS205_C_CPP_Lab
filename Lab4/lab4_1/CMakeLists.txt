cmake_minimum_required(VERSION 3.10)

project(stuinfo)

aux_source_directory(. srcs)

add_executable(stuinfo.out ${srcs})