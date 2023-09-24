#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Función para dibujar una circunferencia por el algoritmo de Bresenham
void drawCircle(int radius, int centerX, int centerY) {
    glPointSize(1.0);
    glBegin(GL_POINTS);

    int x = radius;
    int y = 0;
    int p = 1 - radius;

    while (x >= y) {
        // Calcular las coordenadas en relación al centro
        glVertex2i(centerX + x, centerY + y);
        glVertex2i(centerX + y, centerY + x);
        glVertex2i(centerX - y, centerY + x);
        glVertex2i(centerX - x, centerY + y);
        glVertex2i(centerX - x, centerY - y);
        glVertex2i(centerX - y, centerY - x);
        glVertex2i(centerX + y, centerY - x);
        glVertex2i(centerX + x, centerY - y);

        y++;

        if (p <= 0) {
            p += 2 * y + 1;
        } else {
            x--;
            p += 2 * (y - x) + 1;
        }
    }

    glEnd();
}

int main(int argc, char** argv) {
    // Variables para el radio y las coordenadas del centro, con valores predeterminados
    int radio = (3 * std::min(800, 600)) / 8;
    int centerX = 0;
    int centerY = 0;

    // Verificar si se proporcionaron argumentos para el radio y las coordenadas del centro
    if (argc > 1) {
        // Obtener el radio desde el primer argumento
        radio = std::atoi(argv[1]);
        
        // Obtener las coordenadas del centro si se proporcionaron
        if (argc > 3) {
            centerX = std::atoi(argv[2]);
            centerY = std::atoi(argv[3]);
        }
    }

    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Crear una ventana GLFW de 800x600 (3/4 partes de 800x800)
    GLFWwindow* window = glfwCreateWindow(800, 600, "Circunferencia por Bresenham", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Establecer la ventana como contexto actual
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return -1;
    }

    // Configurar la vista ortográfica
    glOrtho(-400, 400, -300, 300, -1, 1);

    // Ciclo principal
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar la circunferencia con el radio y las coordenadas del centro especificados
        glColor3f(1.0, 1.0, 1.0);  // Color blanco
        drawCircle(radio, centerX, centerY);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminar GLFW
    glfwTerminate();

    return 0;
}
