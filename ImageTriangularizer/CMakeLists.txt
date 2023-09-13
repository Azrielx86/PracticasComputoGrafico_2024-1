cmake_minimum_required(VERSION 3.26)
project(ImageTriangularizer)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )
if (OpenCV_FOUND)
    message(STATUS "OpenCV Found")
endif ()

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(ImageTriangularizer main.cpp)
target_link_libraries( ImageTriangularizer ${OpenCV_LIBS} )