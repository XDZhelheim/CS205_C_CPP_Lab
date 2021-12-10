cmake_minimum_required(VERSION 3.10)

project(CNN)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

include_directories(inc/)
aux_source_directory(src/ srcs)

find_package(OpenCV REQUIRED)
message(STATUS "opencv include path is: " ${OpenCV_INCLUDE_DIRS})
message(STATUS "opencv lib path is: " ${OpenCV_LIBS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main.out ${srcs})
target_link_libraries(main.out ${OpenCV_LIBS})
