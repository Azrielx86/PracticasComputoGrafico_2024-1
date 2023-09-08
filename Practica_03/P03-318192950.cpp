#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
// semestres 2024-1 pr�ctica 3: Modelado Geom�trico y C�mara Sint�tica.
#ifdef CMAKE_PROJECT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#else
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\random.hpp>
#include <gtc\type_ptr.hpp>
#endif
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include <cmath>
#include <map>
// tecla E: Rotar sobre el eje X
// tecla R: Rotar sobre el eje Y
// tecla T: Rotar sobre el eje Z

using std::vector;

// Dimensiones de la ventana
// const float toRadians = 3.14159265f / 180.0; // grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh *> meshList;
vector<Shader> shaderList;
// Vertex Shader
static const char *vShader = "shaders/shader.vert";
static const char *fShader = "shaders/shader.frag";
static const char *vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); // recibe radio, slices, stacks

std::map<std::string, Mesh *> meshMap;
std::map<std::string, Shader> shaderMap;

void CreatePlane()
{
	// clang-format off
	unsigned int plane_idx[] = {
	    0,1,2,
	    1,2,3,
	};

	GLfloat plane_vertex[] = {
	    5.0f, 0.0f, 5.0f,
	    5.0f, 0.0f, -5.0f,
	    -5.0f, 0.0f, 5.0f,
	    -5.0f, 0.0f, -5.0f
	};
	// clang-format on

	Mesh *plane = new Mesh();
	plane->CreateMesh(plane_vertex, plane_idx, 12, 6);
	meshMap["Plane"] = plane;
}

void CreatePyramid()
{
	// clang-format off
	std::vector<GLfloat> vertex = {
// 		x				z				y
	    -0.866025f,  	-0.007107f,		0.025708f,
	    0.0f,        	1.40711f,	    0.525708f,
	    0.866025f,   	-0.007107f,		0.025708f,
		0,           	-0.007107f,		1.52571f,
	};
	
	std::vector<unsigned int> idx = {
	    0, 1, 2,
		0, 1, 3,
		0, 2, 3,
		1, 2, 3,
	};
	// clang-format on

	auto pyramid = new Mesh();
	pyramid->CreateMeshGeometry(vertex, idx, vertex.size(), idx.size());
	meshMap["Pyramid"] = pyramid;
}

void CreateShaders()
{
	auto shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	shaderMap["Shader"] = *shader1;

	auto shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
	shaderMap["ShaderColor"] = *shader2;
}

glm::vec3 ColorFromRGB(int r, int g, int b)
{
	float rn = r / 255.0f;
	float rg = g / 255.0f;
	float rb = b / 255.0f;
	return {rn, rg, rb};
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	// Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CreatePyramid();
	CreateShaders();
	CreatePlane();

#if __linux__
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.6f, 0.8f);
#else
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
#endif

	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformColor;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	// glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	// Loop mientras no se cierra la ventana
	sp.init(); // inicializar esfera
	sp.load(); // enviar la esfera al shader

	glm::mat4 model(1.0); // Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); // inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		// Recibir eventos del usuario
		glfwPollEvents();
		// C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Se agrega limpiar el buffer de profundidad
		shaderMap["Shader"].useShader();
		uniformModel = shaderMap["Shader"].getModelLocation();
		uniformProjection = shaderMap["Shader"].getProjectLocation();
		uniformView = shaderMap["Shader"].getViewLocation();
		uniformColor = shaderMap["Shader"].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// Piramides
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3 - i; j++) // Altura
			{
				for (int k = 0; k < 3 - j - i; k++) // Profundidad
				{
					float x = i + (0.9 * i) + (0.9 * j) + (0.9 * k);
					float y = j + (0.7 * j); // Altura
					float z = k + (0.7 * k) + (0.7 * j); // Profundidad
					model = glm::mat4(1.0);
					model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
					model = glm::translate(model, glm::vec3(x, y, z));
					color = ColorFromRGB(i * 120, j * 120, k * 120);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					meshMap["Pyramid"]->RenderMeshGeometry();
				}
			}
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(-2.0f, 1.0f, -2.0f));
		color = ColorFromRGB(220, 220, 220);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshMap["Plane"]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

#pragma clang diagnostic pop