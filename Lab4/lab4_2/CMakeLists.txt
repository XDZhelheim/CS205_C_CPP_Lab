cmake_minimum_required(VERSION 3.10)

project(Dayinfo)

aux_source_directory(. srcs)

add_executable(dayinfo.out ${srcs})