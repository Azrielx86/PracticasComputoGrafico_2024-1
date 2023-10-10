#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	linternaCamara = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	// Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	// Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 07: Iluminacion 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	// Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	// MANEJAR TECLADO y MOUSE
	createCallbacks();

	// permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // HABILITAR BUFFER DE PROFUNDIDAD
	                         //  Asignar valores de la ventana y coordenadas

	// Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	// Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow *window, int key, int code, int action, int mode)
{
	Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex -= 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex += 1.0;
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		theWindow->linternaCamara = !theWindow->linternaCamara;

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		theWindow->lampara = !theWindow->lampara;

	if (key == GLFW_KEY_H)
		theWindow->mueveHelicopteroX -= 1.0f;
	if (key == GLFW_KEY_K)
		theWindow->mueveHelicopteroX += 1.0f;
	if (key == GLFW_KEY_U)
		theWindow->mueveHelicopteroY += 1.0f;
	if (key == GLFW_KEY_J)
		theWindow->mueveHelicopteroY -= 1.0f;

	if (key == GLFW_KEY_UP)
	{
		theWindow->posVehiculo += 0.2f;
		theWindow->rotaRuedas += 25.0f;
	}

	if (key == GLFW_KEY_DOWN)
	{
		theWindow->posVehiculo -= 0.2f;
		theWindow->rotaRuedas -= 25.0f;
	}

	if (key == GLFW_KEY_N)
	{
		if (theWindow->anguloCofre <= 45)
			theWindow->anguloCofre += 3.0f;
	}

	if (key == GLFW_KEY_M)
	{
		if (theWindow->anguloCofre > 0)
			theWindow->anguloCofre -= 3.0f;
	}

	// Liberar mouse
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		theWindow->toggleMouse = !theWindow->toggleMouse;
		theWindow->mouseMode();
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			// printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			// printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow *window, double xPos, double yPos)
{
	Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
	if (!theWindow->toggleMouse)
		return;

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

GLfloat Window::getPosVehiculo() const
{
	return posVehiculo;
}

GLfloat Window::getRotaRuedas() const
{
	return rotaRuedas;
}

GLfloat Window::getAnguloCofre() const
{
	return anguloCofre;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
