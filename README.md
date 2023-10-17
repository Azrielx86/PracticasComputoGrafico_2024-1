# Prácticas de Cómputo Gráfico e Interacción Humano Computadora

Semestre 2024-1 - FI UNAM

## Configuración del entorno

Argumentos para CMake.

    -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake

Es necesario tener vcpkg en Windows.

    vcpkg install assimp:x64-windows glew:x64-windows glfw3:x64-windows glm:x64-windows
    vcpkg integrate install

Recomendado utilizar LLVM Clang como compilador.

En Linux únicamente no se puede usar `imgui` (o es necesario usar vcpkg de Linux)

Para un mejor rendimiento, hay que compilar la versión `Release`.
