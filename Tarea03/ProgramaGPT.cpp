#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

namespace po = boost::program_options;

void drawCircle(int radius, int centerX, int centerY)
{
	glPointSize(1.0);
	glBegin(GL_POINTS);

	int x = radius;
	int y = 0;
	int p = 1 - radius;

	while (x >= y)
	{
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
	// Declarar variables para las coordenadas (x, y) del centro del círculo, con valores predeterminados (0, 0)
	int x = 0;
	int y = 0;
	int radio = (3 * std::min(800, 600)) / 8;

	try
	{
		// Configurar las opciones de línea de comandos
		po::options_description desc("Opciones");
		desc.add_options()("help", "Mostrar ayuda")("x", po::value<int>(&x)->default_value(0), "Coordenada X del centro del círculo")("y", po::value<int>(&y)->default_value(0), "Coordenada Y del centro del círculo")("radio,r", po::value<int>(&radio)->default_value(radio), "Radio del círculo");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// Verificar si se proporcionó el argumento "help"
		if (vm.count("help"))
		{
			std::cout << desc << std::endl; // Mostrar instrucciones
			return 0;
		}

		// Resto del código como se indicó anteriormente
		// ...
	}
	catch (const po::error &ex)
	{
		std::cerr << "Error al procesar los argumentos: " << ex.what() << std::endl;
		std::cout << "Utiliza --help para ver las instrucciones." << std::endl;
		return 1;
	}

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

		// Dibujar la circunferencia con las coordenadas (x, y) especificadas y el radio especificado
		glColor3f(1.0, 1.0, 1.0); // Color blanco
		drawCircle(radio, x, y);

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
