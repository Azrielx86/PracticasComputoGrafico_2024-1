#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
        keyPairs[i] = std::pair<bool, int>(false, GLFW_RELEASE);
        keyStructs[i].action = GLFW_RELEASE;
        keyStructs[i].pressed = false;
        keyStructs[i].firstStroke = false;
    }
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    muevex = 2.0f;
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}
int Window::Initialise()
{
    // Inicializaci�n de GLFW
    if (!glfwInit())
    {
        printf("Fallo inicializar GLFW");
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
    mainWindow = glfwCreateWindow(width, height, "Practica 09: Animacion", NULL, NULL);

    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }
    // Obtener tama�o de Buffer
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // asignar el contexto
    glfwMakeContextCurrent(mainWindow);

    // MANEJAR TECLADO y MOUSE
    createCallbacks();

    // permitir nuevas extensiones
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Fallo inicializaci�n de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST); // HABILITAR BUFFER DE PROFUNDIDAD
                             //  Asignar valores de la ventana y coordenadas

    // Asignar Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
    // Callback para detectar que se est� usando la ventana
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

    // Liberar mouse
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        theWindow->toggleMouse = !theWindow->toggleMouse;
        theWindow->mouseMode();
    }

    /*
     * Controles:
     * M - Modo captura o Modo ejecutar animación.
     * == Modo Captura ==
     * C - Guarda Frame
     * R - Elimina Frame
     * == Modo animacion ==
     * Space - Ejecuta animación.
     * R - Reinicia animación.
     */
    //    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    //        theWindow->controlMode = true;
    //    if (key == GLFW_KEY_M && action == GLFW_RELEASE)
    //        theWindow->controlMode = false;
    //
    //    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    //        theWindow->controlMode = true;
    //    if (key == GLFW_KEY_M && action == GLFW_RELEASE)
    //        theWindow->controlMode = false;
    //
    //    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    //        theWindow->controlMode = true;
    //    if (key == GLFW_KEY_M && action == GLFW_RELEASE)
    //        theWindow->controlMode = false;
    //
    //    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    //        theWindow->controlMode = true;
    //    if (key == GLFW_KEY_M && action == GLFW_RELEASE)
    //        theWindow->controlMode = false;

    // todo : Clase o struct Key
    /*
     * bool active;
     * enum action;
     *
     * De esta crear un arreglo en Input
     *
     * https://stackoverflow.com/questions/46631814/handling-multiple-keys-input-at-once-with-glfw
     */
    
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            theWindow->keyPairs[key].second = theWindow->keyPairs[key].first ? GLFW_REPEAT : GLFW_PRESS;
            theWindow->keyPairs[key].first = true;
            
            // printf("se presiono la tecla %d'\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            theWindow->keyPairs[key].first = false;
            theWindow->keyPairs[key].second = action;
            // printf("se solto la tecla %d'\n", key);
        }
        else
        {
            theWindow->keyPairs[key].second = action;
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

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}