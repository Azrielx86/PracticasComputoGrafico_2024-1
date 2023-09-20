# File by Azrielx86 (Edgar Chalico)
cmake_minimum_required(VERSION 3.7.2)
project(main)
set(CMAKE_CXX_STANDARD 17)

# Requires GLEW, OpenGL and GLFW Packages on Linux
# On Arch Linux: pacman -S glew glfw

# Or Windows require install packages with vcpkg with the following command:
# ./vcpkg install glfw3:x64-windows glfw3:x86-windows glew:x64-windows glew:x86-windows glm:x64-windows glm:x86-windows

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

find_package(assimp CONFIG REQUIRED)
if (assimp_FOUND)
    message(STATUS "Assimp Found")
endif ()


if (NOT UNIX)
    include_directories(C:/dev/vcpkg/packages/glfw3_x86-windows/include/GLFW)
    include_directories(C:/dev/vcpkg/packages/glew_x86-windows/include/GL)
    include_directories(C:/dev/vcpkg/packages/glm_x86-windows/include/glm)
    include_directories(C:/dev/vcpkg/packages/assimp_x86-windows/include)
else()
    include_directories(/usr/include/GLFW)
    include_directories(/usr/include/GL)
    include_directories(/usr/include/glm)
endif ()

add_executable(main E05-318192950.cpp
        Window.cpp
        Camera.cpp
        DirectionalLight.cpp
        Light.cpp
        Material.cpp
        Mesh_tn.cpp
        Model.cpp
        PointLight.cpp
        Shader_m.cpp
        Skybox.cpp
        Sphere.cpp
        SpotLight.cpp
        Texture.cpp
)

    file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY Models DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY Textures DESTINATION ${CMAKE_BINARY_DIR})
if (UNIX)
    target_link_libraries(main glfw OpenGL::GL GLEW)
else ()
    target_link_libraries(main PRIVATE GLEW::GLEW)
    target_link_libraries(main PRIVATE glfw)
    target_link_libraries(main PRIVATE assimp::assimp)
endif ()
