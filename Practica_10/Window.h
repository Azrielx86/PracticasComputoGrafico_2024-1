#pragma once
#include <glew.h>
#include <glfw3.h>
#include <cstdio>
#include <utility>


class Window
{
  public:
    typedef struct _key
    {
        int action;
        bool pressed;
        bool firstStroke;
    } Key;

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
    std::pair<bool, int> *getKeysPairs() { return keyPairs; }
    void swapBuffers() { return glfwSwapBuffers(mainWindow); }
    GLFWwindow *getWindowPointer() { return this->mainWindow; }
    ~Window();

    static constexpr std::pair<bool, int> keyPressed = std::pair<bool, int>(true, GLFW_PRESS);
    static constexpr std::pair<bool, int> keyRepeat = std::pair<bool, int>(true, GLFW_REPEAT);
    static constexpr std::pair<bool, int> keyRelease = std::pair<bool, int>(false, GLFW_RELEASE);

  private:
    GLFWwindow *mainWindow;
    GLint width, height;
    bool keys[1024];
    std::pair<bool, int> keyPairs[1024];
    Key keyStructs[1024];
    GLint bufferWidth, bufferHeight;
    void createCallbacks();
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    GLfloat muevex;
    GLfloat anguloCofre = 0.0f;
    bool deleteFrame = false;
    bool resetAnimation = false;
    bool controlMode = false;
    bool saveFrame = false;

  public:
    /**
     * Tecla O
     */
    [[nodiscard]] bool getResetAnimation() const { return resetAnimation; };

    /**
     * Tecla M
     */
    [[nodiscard]] bool getControlMode() const { return controlMode; }

    /**
     * Tecla C
     */
    [[nodiscard]] bool getSaveFrame() const { return saveFrame; }

    /**
     * Tecla P
     */
    [[nodiscard]] bool getStartCoinAnimation() const { return deleteFrame; };

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
