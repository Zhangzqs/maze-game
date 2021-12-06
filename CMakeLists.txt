cmake_minimum_required(VERSION 3.16)
project(Maze)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Maze
        main.cpp)

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(Maze ${OpenCV_LIBS})