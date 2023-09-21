#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

namespace po = boost::program_options;

// Función para dibujar una circunferencia por el algoritmo de Bresenham
void drawCircle(int radius)
{
	glPointSize(1.0);
	glBegin(GL_POINTS);

	int x = radius;
	int y = 0;
	int p = 1 - radius;

	while (x >= y)
	{
		// Octante 1
		glVertex2i(x, y);
		// Octante 2
		glVertex2i(y, x);
		// Octante 3
		glVertex2i(-y, x);
		// Octante 4
		glVertex2i(-x, y);
		// Octante 5
		glVertex2i(-x, -y);
		// Octante 6
		glVertex2i(-y, -x);
		// Octante 7
		glVertex2i(y, -x);
		// Octante 8
		glVertex2i(x, -y);

		y++;

		if (p <= 0)
		{
			p += 2 * y + 1;
		}
		else
		{
			x--;
			p += 2 * (y - x) + 1;
		}
	}

	glEnd();
}

int main(int argc, char **argv)
{
	// Declarar variables para el radio y el valor predeterminado
	int radio;
	int radioPredeterminado = (3 * std::min(800, 600)) / 8;

	// Configurar las opciones de línea de comandos
	po::options_description desc("Opciones");
	desc.add_options()("radio,r", po::value<int>(&radio)->default_value(radioPredeterminado), "Radio del círculo");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// Inicializar GLFW
	if (!glfwInit())
	{
		std::cerr << "Error al inicializar GLFW" << std::endl;
		return -1;
	}

	// Crear una ventana GLFW de 800x600 (3/4 partes de 800x800)
	GLFWwindow *window = glfwCreateWindow(800, 600, "Circunferencia por Bresenham", NULL, NULL);
	if (!window)
	{
		std::cerr << "Error al crear la ventana GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Establecer la ventana como contexto actual
	glfwMakeContextCurrent(window);

	// Inicializar GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error al inicializar GLEW" << std::endl;
		return -1;
	}

	// Configurar la vista ortográfica
	glOrtho(-400, 400, -300, 300, -1, 1);

	// Ciclo principal
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibujar la circunferencia con el radio especificado
		glColor3f(1.0, 1.0, 1.0); // Color blanco
		drawCircle(radio);

		// Detectar si la tecla ESC está siendo presionada
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminar GLFW
	glfwTerminate();

	return 0;
}
