#pragma once
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>

class Window
{
  public:
	enum Articulation // Enum para las patas
	{
		FRONT_LEFT = 0,
		FRONT_RIGHT = 1,
		BACK_LEFT = 2,
		BACK_RIGHT = 3
	};
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getangulocola() { return angulocola; }
	GLfloat getAnguloPata(Articulation pata) { return anguloPatas[pata]; }
	GLfloat getPosVehiculo() const;
	GLfloat getRotaRuedas() const;
	GLfloat getAnguloCofre() const;
	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	bool *getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
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
	GLfloat angulocola = 0.0f;
	GLfloat anguloPatas[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat posVehiculo = 0.0f;
	GLfloat anguloCofre = 0.0f;
	GLfloat rotaRuedas = 0.0f;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow *window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow *window, double xPos, double yPos);
};
