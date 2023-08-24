# File by Azrielx86 (Edgar Chalico)
cmake_minimum_required(VERSION 3.7.2)
project(main)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Requires GLEW, OpenGL and GLFW Packages on Linux
# On Arch Linux: pacman -S glew glfw

# Or Windows require install packages with vcpkg with the following command:
# ./vcpkg install glfw3:x64-windows glfw3:x86-windows glew:x64-windows glew:x86-windows glm:x64-windows glm:x86-windows

add_definitions(-DCMAKE_PROJECT)
# FIND PACKAGES FOR OpenGL, GLFW and GLEW
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "OpenGL Found")
endif ()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "GLEW Found")
endif ()

find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
    message(STATUS "GLFW Found")
endif ()

add_executable(main P01-318192950.cpp)

if (UNIX)
    target_link_libraries(main glfw OpenGL::GL GLEW)
else ()
    target_link_libraries(main PRIVATE GLEW::GLEW)
    target_link_libraries(main PRIVATE glfw)
endif ()

