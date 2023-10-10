#pragma once
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>

class Window
{
  public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getMueveHelicopteroX() const { return mueveHelicopteroX; }
	GLfloat getMueveHelicopteroY() const { return mueveHelicopteroY; }
	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	bool *getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLFWwindow *getWindowPointer() { return this->mainWindow; }
	bool getLinterna() const { return this->linternaCamara; }
	bool getLampara() const { return this->lampara; }
	GLfloat getPosVehiculo() const;
	GLfloat getRotaRuedas() const;
	GLfloat getAnguloCofre() const;

	~Window();

  private:
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat mueveHelicopteroX = 0.0f;
	GLfloat mueveHelicopteroY = 10.0f;
	GLfloat posVehiculo = 0.0f;
	GLfloat anguloCofre = 0.0f;
	GLfloat rotaRuedas = 0.0f;
	bool linternaCamara = true, lampara = true;
	bool toggleMouse = true;
	void mouseMode()
	{
		if (this->toggleMouse)
			glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow *window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow *window, double xPos, double yPos);
};
