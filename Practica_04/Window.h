#pragma once
#pragma warning(push, 0)
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
	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	bool *getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	/**
	 * @brief Tecla F
	 */
	GLfloat getarticulacion1() { return articulacion1; }
	/**
	 * @brief Tecla G
	 */
	GLfloat getarticulacion2() { return articulacion2; }
	/**
	 * @brief Tecla H
	 */
	GLfloat getarticulacion3() { return articulacion3; }
	/**
	 * @brief Tecla J
	 */
	GLfloat getarticulacion4() { return articulacion4; }
	/**
	 * @brief Tecla K
	 */
	GLfloat getarticulacion5() { return articulacion5; }
	/**
	 * @brief Tecla K
	 */
	GLfloat getarticulacion6() { return articulacion6; }
	/**
	 * @brief Tecla V
	 */
	GLfloat getarticulacion7() { return articulacion7; }
	/**
	 * @brief Tecla B
	 */
	GLfloat getarticulacion8() { return articulacion8; }
	/**
	 * @brief Tecla N
	 */
	GLfloat getarticulacion9() { return articulacion9; }
	/**
	 * @brief Tecla M
	 */
	GLfloat getarticulacion10() { return articulacion10; }

	~Window();

  private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7, articulacion8, articulacion9, articulacion10;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow *window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow *window, double xPos, double yPos);
};

#pragma warning(pop)