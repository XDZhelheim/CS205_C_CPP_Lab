cmake_minimum_required(VERSION 3.10)

project(CNN)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-DNDBUG ${CMAKE_CXX_FLAGS}")

include_directories(inc/)
aux_source_directory(src/ srcs)

find_package(OpenCV REQUIRED)
message(STATUS "opencv include path is: " ${OpenCV_INCLUDE_DIRS})
message(STATUS "opencv lib path is: " ${OpenCV_LIBS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(face_detect ${srcs})
target_link_libraries(face_detect ${OpenCV_LIBS})
