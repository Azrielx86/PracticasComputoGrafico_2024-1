// Moreno Chalico Edgar Ulises
#ifdef CMAKE_PROJECT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <glew.h>
#include <glfw3.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

// Vertex Shader
// recibir color, salida Vcolor
static const char *vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
// recibir Vcolor y dar de salida color
static const char *fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,1.0f,1.0f);	 			\n\
}";

void CrearTriangulo()
{
	GLfloat vertices[] = {
	    // ===== Letra E =====
	    // Borde izquierdo
	    -0.8f, // A Primer triangulo
	    0.6f,
	    0.0f,
	    -0.7f, // B
	    0.6f,
	    0.0f,
	    -0.8f, // C
	    -0.6f,
	    0.0f,

	    -0.7f, // B Segundo triangulo
	    0.6f,
	    0.0f,
	    -0.8f, // C
	    -0.6f,
	    0.0f,
	    -0.7f, // D
	    -0.6f,
	    0.0f,
	    // Primer bloque superior
	    -0.7f, // B
	    0.6f,
	    0.0f,
	    -0.7f, // H
	    0.4f,
	    0.0f,
	    -0.6f, // J
	    0.6f,
	    0.0f,

	    -0.7f, // H
	    0.4f,
	    0.0f,
	    -0.6f, // J
	    0.6f,
	    0.0f,
	    -0.6f, // I
	    0.4f,
	    0.0f,
	    // Bloque medio
	    -0.7f, // K
	    0.1f,
	    0.0f,
	    -0.7f, // N
	    -0.15f,
	    0.0f,
	    -0.6f, // L
	    0.1f,
	    0.0f,

	    -0.7f, // N
	    -0.15f,
	    0.0f,
	    -0.6f, // L
	    0.1f,
	    0.0f,
	    -0.6f, // M
	    -0.15f,
	    0.0f,
	    // Bloque inferior
	    -0.7f, // G
	    -0.4f,
	    0.0f,
	    -0.7f, // D
	    -0.6f,
	    0.0f,
	    -0.6f, // F
	    -0.4f,
	    0.0f,

	    -0.7f, // D
	    -0.6f,
	    0.0f,
	    -0.6f, // F
	    -0.4f,
	    0.0f,
	    -0.6f, // E
	    -0.6f,
	    0.0f,

	    // ===== Letra U =====
	    // Bloque izquierdo
	    -0.4f, // O
	    0.6f,
	    0.0f,
	    -0.3f, // P
	    0.6f,
	    0.0f,
	    -0.4f, // Q,
	    -0.4f,
	    0.0f,

	    -0.3f, // P
	    0.6f,
	    0.0f,
	    -0.4f, // Q
	    -0.4f,
	    0.0f,
	    -0.3f, // R
	    -0.4f,
	    0.0f,

	    // Segmento inferior izquierdo
	    -0.4f, // Q
	    -0.4f,
	    0.0f,
	    -0.3f, // R
	    -0.4f,
	    0.0f,
	    -0.3f, // S
	    -0.6f,
	    0.0f,

	    // Bloque derecho
	    -0.2f, // W
	    0.6f,
	    0.0f,
	    -0.1f, // Z
	    0.6f,
	    0.0f,
	    -0.2f, // V
	    -0.4f,
	    0.0f,

	    -0.1f, // Z
	    0.6f,
	    0.0f,
	    -0.2f, // V
	    -0.4f,
	    0.0f,
	    -0.1f, // U
	    -0.4f,
	    0.0f,

	    // Segmento inferior derecho
	    -0.2f, // V
	    -0.4f,
	    0.0f,
	    -0.1f, // U
	    -0.4f,
	    0.0f,
	    -0.2f, // T
	    -0.6f,
	    0.0f,

	    // Segmento inferior
	    -0.3f, // R
	    -0.4f,
	    0.0f,
	    -0.3f, // S
	    -0.6f,
	    0.0f,
	    -0.2f, // V
	    -0.4f,
	    0.0f,

	    -0.3f, // S
	    -0.6f,
	    0.0f,
	    -0.2f, // V
	    -0.4f,
	    0.0f,
	    -0.2f, // T
	    -0.6f,
	    0.0f,

	    // ===== Letra M =====
	    // Bloque lateral izquierdo
	    0.1f, // A1
	    0.6f,
	    0.0f,
	    0.1f, // B1
	    -0.6f,
	    0.0f,
	    0.2f, // D1,
	    0.6f,
	    0.0f,

	    0.1f, // B1
	    -0.6f,
	    0.0f,
	    0.2f, // D1,
	    0.6f,
	    0.0f,
	    0.2f, // C1
	    -0.6f,
	    0.0f,

	    // Bloque lateral derecho
	    0.3f, // E1
	    0.6f,
	    0.0f,
	    0.4f, // F1
	    0.6f,
	    0.0f,
	    0.3f, // G1
	    -0.6f,
	    0.0f,

	    0.4f, // F1
	    0.6f,
	    0.0f,
	    0.3f, // G1
	    -0.6f,
	    0.0f,
	    0.4f, // H1
	    -0.6f,
	    0.0f,

	    // Diagonal izquierda
	    0.2f, // D1,
	    0.6f,
	    0.0f,
	    0.2f, // S1
	    0.4f,
	    0.0f,
	    0.25f, // V1
	    0.35f,
	    0.0f,

	    0.2f, // D1,
	    0.6f,
	    0.0f,
	    0.25f, // V1
	    0.35f,
	    0.0f,
	    0.25f, // U1
	    0.55f,
	    0.0f,

	    // Diagonal derecha
	    0.3f, // E1
	    0.6f,
	    0.0f,
	    0.25f, // V1
	    0.35f,
	    0.0f,
	    0.25f, // U1
	    0.55f,
	    0.0f,

	    0.3f, // E1
	    0.6f,
	    0.0f,
	    0.25f, // V1
	    0.35f,
	    0.0f,
	    0.3f, // T1
	    0.4f,
	    0.0f,

	    // ===== Letra C =====
	    // Bloque Izquierdo
	    0.6f, // I1
	    0.6f,
	    0.0f,
	    0.7f, // M1
	    0.6f,
	    0.0f,
	    0.6f, // N1
	    -0.6f,
	    0.0f,

	    0.7f, // M1
	    0.6f,
	    0.0f,
	    0.6f, // N1
	    -0.6f,
	    0.0f,
	    0.7f, // O1
	    -0.6f,
	    0.0f,

	    // Bloque superior
	    0.7f, // M1
	    0.6f,
	    0.0f,
	    0.8f, // J1
	    0.6f,
	    0.0f,
	    0.7f, // L1
	    0.4f,
	    0.0f,

	    0.8f, // J1
	    0.6f,
	    0.0f,
	    0.7f, // L1
	    0.4f,
	    0.0f,
	    0.8f, // K1
	    0.4f,
	    0.0f,

	    // Bloque inferior
	    0.7f, // O1
	    -0.6f,
	    0.0f,
	    0.7f, // R1
	    -0.4f,
	    0.0f,
	    0.8f, // Q1
	    -0.4f,
	    0.0f,

	    0.7f, // O1
	    -0.6f,
	    0.0f,
	    0.8f, // Q1
	    -0.4f,
	    0.0f,
	    0.8f, // P1
	    -0.6f,
	    0.0f,
	};
	glGenVertexArrays(1, &VAO); // generar 1 VAO
	glBindVertexArray(VAO);     // asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
	             GL_STATIC_DRAW); // pasarle los datos al VBO asignando tamano, los datos y en este caso es est�tico pues no se modificar�n los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *) 0); // Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos NOLINT(*-use-nullptr, *-sizeof-expression)
	glEnableVertexAttribArray(0);
	// agregar valores a v�rtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// the Program recibe los datos de theShader
void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType) // Funci�n para agregar los shaders a la tarjeta gr�fica
{
	GLuint theShader = glCreateShader(shaderType); // theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar *theCode[1];
	theCode[0] = shaderCode; // shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);                // longitud del texto NOLINT(*-narrowing-conversions)
	glShaderSource(theShader, 1, theCode, codeLength); // Se le asigna al shader el c�digo
	glCompileShader(theShader);                        // Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = {0};
	// verificaciones y prevenci�n de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader); // Si no hubo problemas se asigna el shader a theProgram el cual asigna el c�digo a la tarjeta gr�fica
}

void CompileShaders()
{
	shader = glCreateProgram(); // se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);   // Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER); // Agregar fragment shader
	// Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = {0};
	glLinkProgram(shader); // se linkean los shaders a la tarjeta gr�fica
	// verificaciones y prevenci�n de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

/**
 * Asigna valores aleatorios de rojo, verde y azul.
 * @param r Puntero al valor de rojo
 * @param g Puntero al valor de verde
 * @param b Puntero al valor de azul
 */
void assign_random_color(float *r, float *g, float *b)
{
	*r = rand() % 255 / 255.0f; // NOLINT(*-narrowing-conversions, *-msc50-cpp)
	*g = rand() % 255 / 255.0f; // NOLINT(*-narrowing-conversions, *-msc50-cpp)
	*b = rand() % 255 / 255.0f; // NOLINT(*-narrowing-conversions, *-msc50-cpp)
}

int main()
{
	srand(time(nullptr)); // NOLINT(*-msc51-cpp)
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
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", nullptr, nullptr);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	// Obtener tama�o de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	// asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	// permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo inicializacion de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	// Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	// Crear triangulo
	CrearTriangulo();
	CompileShaders();

	float r, g, b;
	assign_random_color(&r, &g, &b);

	int current_time, prev_time = 0;

	// Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		current_time = (int) glfwGetTime(); // Se obtiene el tiempo  actual en ejecución
		// Impresion para ver el conteo en segundos
		printf("Tiempo transcurrido: %d segundos.\r", current_time);
		if (current_time > prev_time && current_time % 3 == 0) // Se realiza el cambio cada 3 segundos.
		{
			prev_time = current_time; // Tiempo anterior = Tiempo actual.
			assign_random_color(&r, &g, &b); // Se llama a una funcion para asignar el color.
			// Impresión para mostrar el conteo y el color asignado.
			printf("Color: (%.2f, %.2f, %.2f) [%d segundos]\n", r, g, b, current_time);
		}

		// Recibir eventos del usuario
		glfwPollEvents();

		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 270);
		glBindVertexArray(0);

		glUseProgram(0);

		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == 1)
			break;

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}