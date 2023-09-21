cmake_minimum_required(VERSION 3.26)
project(ImageTriangularizer)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )
if (OpenCV_FOUND)
    message(STATUS "OpenCV Found")
endif ()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(ImageTriangularizer main.cpp)
target_link_libraries( ImageTriangularizer ${OpenCV_LIBS} OpenMP::OpenMP_CXX )