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
    bool getShouldClose()
    {
        return glfwWindowShouldClose(mainWindow);
    }
    bool *getsKeys() { return keys; }
    void swapBuffers() { return glfwSwapBuffers(mainWindow); }
    GLFWwindow *getWindowPointer() { return this->mainWindow; }
    [[nodiscard]] GLfloat getAnguloCofre() const;
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
    GLfloat anguloCofre = 0.0f;
    bool startCoinAnimation = false;
    bool resetAnimation = false;
    bool startHelicAnimation = false;
    bool startDiceAnimation = false;

  public:
    /**
     * Tecla O 
     */
    [[nodiscard]] bool getResetAnimation() const;
    
    /**
     * Tecla H 
     */
    [[nodiscard]] bool getStartHelicAnimation() const { return startHelicAnimation; }
    
    /**
     * Tecla U 
     */
    [[nodiscard]] bool getStartDiceAnimation() const { return startDiceAnimation; }
    
    /**
     * Tecla P
     */
    [[nodiscard]] bool getStartCoinAnimation() const;

  private:
    bool toggleMouse = true;
    void mouseMode()
    {
        if (this->toggleMouse)
            glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    bool mouseFirstMoved = true;
    static void ManejaTeclado(GLFWwindow *window, int key, int code, int action, int mode);
    static void ManejaMouse(GLFWwindow *window, double xPos, double yPos);
};
