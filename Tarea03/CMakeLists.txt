# File by Azrielx86 (Edgar Chalico)
cmake_minimum_required(VERSION 3.7.2)
project(Tarea03)
set(CMAKE_CXX_STANDARD 17)

# Requires GLEW, OpenGL and GLFW Packages on Linux
# On Arch Linux: pacman -S glew glfw boost

# Or Windows require install packages with vcpkg with the following command:
# ./vcpkg install glfw3:x64-windows glfw3:x86-windows glew:x64-windows glew:x86-windows glm:x64-windows glm:x86-windows boost

# FIND PACKAGES FOR OpenGL, GLFW and GLEW
set(OpenGL_GL_PREFERENCE GLVND)
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

find_package(Boost COMPONENTS program_options REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost Found")
endif ()

include_directories(${Boost_INCLUDE_DIR})

add_executable(Tarea03 ProgramaGPT_01.cpp)

if (UNIX)
    target_link_libraries(Tarea03 glfw OpenGL::GL GLEW ${Boost_LIBRARIES})
else ()
    target_link_libraries(Tarea03 PRIVATE GLEW::GLEW)
    target_link_libraries(Tarea03 PRIVATE glfw)
    target_link_libraries(Tarea03 PRIVATE ${Boost_LIBRARIES})
endif ()
