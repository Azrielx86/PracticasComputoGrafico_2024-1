#pragma once
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>

class Window
{
  public:
	enum CAR_LIGHT
	{
		FRONT_LIGHT,
		BACK_LIGHT
	};

	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	[[nodiscard]] GLfloat getmuevex() const { return muevex; }
	[[nodiscard]] GLfloat getMueveHelicopteroX() const { return mueveHelicopteroX; }
	[[nodiscard]] GLfloat getMueveHelicopteroY() const { return mueveHelicopteroY; }
	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	bool *getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLFWwindow *getWindowPointer() { return this->mainWindow; }
	[[nodiscard]] bool getLinterna() const { return this->linternaCamara; }
	[[nodiscard]] bool getLampara() const { return this->lampara; }
	[[nodiscard]] GLfloat getPosVehiculo() const;
	[[nodiscard]] GLfloat getRotaRuedas() const;
	[[nodiscard]] GLfloat getAnguloCofre() const;
	[[nodiscard]] CAR_LIGHT getCurrentCarLight() const;
	[[nodiscard]] int getTiempoHelicoptero() const;
	void setTiempoHelicoptero(int th);
	[[nodiscard]] bool getToggleLuzHelicoptero() const { return toggleLuzHelicoptero; };
	[[nodiscard]] bool getKeyHelicoptero() const { return keyHelicoptero; }
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
	CAR_LIGHT currentLight;
	int tiempoHelicoptero;
	bool toggleLuzHelicoptero = true;
	bool keyHelicoptero = false;
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
