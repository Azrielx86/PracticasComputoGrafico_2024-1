cmake_minimum_required(VERSION 3.7.2)
project(Practica09)
set(CMAKE_CXX_STANDARD 17)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if (DEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    elseif (RELEASE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif ()
endif ()

# Requires GLEW, OpenGL and GLFW Packages on Linux
# On Arch Linux: pacman -S glew glfw

# Or Windows require install packages with vcpkg with the following command:
# ./vcpkg install glfw3:x64-windows glfw3:x86-windows glew:x64-windows glew:x86-windows glm:x64-windows glm:x86-windows assimp

# PROBLEMAS DE RENDIMIENTO DE ASSIMP
# https://gamedev.stackexchange.com/questions/63498/assimp-in-my-program-is-much-slower-to-import-than-assimp-view-program
# Para evitar las largas cargas al inicio del programa, es necesario compilar con la configuraci�n RELEASE

# Para compilar con Clang (12 la versión de VS)
# Compilador: C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\Llvm\bin\clang.exe
# O usar la versión de LLVM (Clang 17) (Recomendado).

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "OpenGL Found")
endif ()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "GLEW Found.")
endif ()

find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
    message(STATUS "GLFW Found.")
endif ()

find_package(assimp CONFIG REQUIRED)
if (assimp_FOUND)
    message(STATUS "Assimp Found.")
endif ()

# vcpkg install imgui[glfw-binding,opengl3-binding]
#find_package(imgui CONFIG REQUIRED)
#if (imgui_FOUND)
#    message(STATUS "InGui Found.")
#endif ()

if (NOT UNIX)
    include_directories(C:/dev/vcpkg/packages/glfw3_x86-windows/include/GLFW)
    include_directories(C:/dev/vcpkg/packages/glew_x86-windows/include/GL)
    include_directories(C:/dev/vcpkg/packages/glm_x86-windows/include/glm)
    include_directories(C:/dev/vcpkg/packages/assimp_x86-windows/include)
else ()
    include_directories(/usr/include/GLFW)
    include_directories(/usr/include/GL)
    include_directories(/usr/include/glm)
    include_directories(/usr/include/stb)
endif ()

add_executable(Practica09 P09-318192950.cpp
        Window.cpp
        Camera.cpp
        DirectionalLight.cpp
        Light.cpp
        Material.cpp
        Mesh_tn.cpp
        Model.cpp
        PointLight.cpp
        Shader_light.cpp
        Skybox.cpp
        Sphere.cpp
        SpotLight.cpp
        Texture.cpp
        ModelMatrix.cpp
        LightCollection.cpp
        LightCollection.h
        Animation.cpp
        Animation.h
)

message(STATUS "Copiando archivos de shader")
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "Copiando archivos de modelos")
file(COPY Models DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "Copiando archivos de texturas")
file(COPY Textures DESTINATION ${CMAKE_BINARY_DIR})
if (UNIX)
    target_link_libraries(Practica09 glfw OpenGL::GL GLEW assimp::assimp)
else ()
    target_link_libraries(Practica09 PRIVATE GLEW::GLEW)
    target_link_libraries(Practica09 PRIVATE glfw)
    target_link_libraries(Practica09 PRIVATE assimp::assimp)
    #    target_link_libraries(Practica09 PRIVATE imgui::imgui)
endif ()
