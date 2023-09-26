# File by Azrielx86 (Edgar Chalico)
cmake_minimum_required(VERSION 3.7.2)
project(CharacterDraw)
set(CMAKE_CXX_STANDARD 17)

# Requires GLEW, OpenGL and GLFW Packages on Linux
# On Arch Linux: pacman -S glew glfw

# Or Windows require install packages with vcpkg with the following command:
# ./vcpkg install glfw3:x64-windows glfw3:x86-windows glew:x64-windows glew:x86-windows glm:x64-windows glm:x86-windows

#add_definitions(-DCMAKE_PROJECT)
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

if (NOT UNIX)
    include_directories(C:/dev/vcpkg/packages/glfw3_x86-windows/include/GLFW)
    include_directories(C:/dev/vcpkg/packages/glew_x86-windows/include/GL)
    include_directories(C:/dev/vcpkg/packages/glm_x86-windows/include/glm)
else()
    include_directories(/usr/include/GLFW)
    include_directories(/usr/include/GL)
    include_directories(/usr/include/glm)
endif ()

add_executable(CharacterDraw
        main.cpp
        Shader.cpp
        MeshColor.cpp
        Window.cpp
        ObjectReader.cpp)

if (UNIX)
    target_link_libraries(CharacterDraw glfw OpenGL::GL GLEW)
else ()
    target_link_libraries(CharacterDraw PRIVATE GLEW::GLEW)
    target_link_libraries(CharacterDraw PRIVATE glfw)
endif ()

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ObjParsed DESTINATION ${CMAKE_BINARY_DIR})
