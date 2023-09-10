#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
// semestres 2024-1 práctica 3: Modelado Geométrico y Cámara Sintética.
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

void CreatePolygon()
{
	// clang-format off
	std::vector<GLfloat> vertex = {
// 		x				z				y
//		Cara superior
		0.0f,			0.707107f, 		1.0f,  // Superior		[0]
		-0.866025,		0.707107f, 		-0.5f, // Izquierda		[1]
		0.866025f,		0.707107,		-0.5f, // Derecha		[2]
// 		Cara inferior
		0.0f,			-0.707107f,		-1.0f, // Inferior		[3]
		-0.866025f, 	-0.707107f,		0.5f,  // Izquierda		[4]
		0.866025f,		-0.707107f,		0.5f,  // Derecha		[5]
	};
	
	std::vector<unsigned int> idx = {
		0, 1, 2, // Cara superior
		0, 4, 5, // Lateral Superior
		0, 4, 1, // Lateral Izquierdo +
		0, 5, 2, // Lateral Derecho +
		3, 4, 5, // Cara inferior
		3, 1, 2, // Lateral Inferior
		1, 4, 3, // Lateral Izquierdo -
		3, 2, 5, // Lateral Derecho -
	};
	// clang-format on

	auto polygon = new Mesh();
	polygon->CreateMeshGeometry(vertex, idx, vertex.size(), idx.size());
	meshMap["Polygon"] = polygon;
}

void CreateTriangle()
{
	// clang-format off
	std::vector<GLfloat> vertex = {
// 		x				z				y
//		-0.69282f, 		0.030897f, 		-0.399999f,
//		0.69282f,		0.030897f,		-0.399999f,
//		0.0f,			0.030897f,		0.799996f,
//
//		-0.69282f, 		-0.030897f, 	-0.399999f,
//		0.69282f,		-0.030897f,		-0.399999f,
//		0.0f,			-0.030897f,		0.799996f,
		
// 		Cara superior
		-0.69282f, 		-0.005718f, 	0.147714f, // Izquierdo		[0]
		0.69282f, 		-0.005718f, 	0.147714f, // Derecho		[1]
		0.0f,			-0.005718f, 	1.34771f,  // Superior		[2]
//		Cara inferior
		-0.69282f, 		-0.067512f, 	0.147714f, // -Izquierdo	[3]
		0.69282f, 		-0.067512f,		0.147714f, // -Derecho		[4]
		0.0f,			-0.067512f, 	1.34771f,  // -Superior		[5]
	};
	
	std::vector<unsigned int> idx = {
// Caras
		0, 1, 2, 
		3, 4, 5,
// Borde inferior
		0, 1, 4,
		0, 3, 4,
// Borde derecho
		2, 1, 4,
		2, 4, 5,
// Borde izquierdo
		2, 0, 5,
		0, 3, 5,
	};
	// clang-format on

	auto triangle = new Mesh();
	triangle->CreateMeshGeometry(vertex, idx, vertex.size(), idx.size());
	meshMap["Triangle"] = triangle;
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
	// Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CreatePyramid();
	CreatePolygon();
	CreateShaders();
	CreatePlane();
	CreateTriangle();

#if __linux__
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.4f, 0.8f);
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
		// Cámara
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

#ifndef XD
		// Piramides
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3 - i; j++) // Altura
			{
				for (int k = 0; k < 3 - j - i; k++) // Profundidad
				{
					float x = i + (0.9 * i) + (0.9 * j) + (0.9 * k);
					float y = j + (0.6 * j);             // Altura
					float z = k + (0.7 * k) + (0.7 * j); // Profundidad
					model = glm::mat4(1.0);
					model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
					model = glm::translate(model, glm::vec3(x, y, z));
					color = ColorFromRGB(i * 160, j * 160, k * 160);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					meshMap["Pyramid"]->RenderMesh();
				}
			}
		}

		// Polígonos
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2 - i; j++) // Altura
			{
				for (int k = 0; k < 2 - j - i; k++) // Profundidad
				{
					float x = i + (0.9 * i) + (0.9 * j) + (0.9 * k) + 0.95;
					float y = j + (0.6 * j) + 0.7;             // Altura
					float z = k + (0.7 * k) + (0.7 * j) + 1.1; // Profundidad
					model = glm::mat4(1.0);
					model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
					model = glm::translate(model, glm::vec3(x, y, z));
					color = ColorFromRGB(i * 180, j * 180, k * 180);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					meshMap["Polygon"]->RenderMesh();
				}
			}
		}
#endif
		
		float rotate = 0.0f;
		for (int i = 0; i < 5; i+=2)
		{
			rotate = 0.0f;
//			for (int j = 0; j < 3 - i; j++) // Altura
//			{
				for (int k = 0; k < 5 - i; k++) // Profundidad
				{
					float x = (i >> 1) + (0.9 * (i >> 1)) + (0.9 * (k / 2.0f)) + ((rotate != 0) ? 0.45f : 0.0f);
					float y = 0;             // Altura
					float z = (k / 2.0f) + (0.7 * (k / 2.0f)) + ((rotate != 0) ? 0.8f : 0.0f); // Profundidad
					model = glm::mat4(1.0);
					model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
					model = glm::translate(model, glm::vec3(x, y, z));
//				    if (rotate != 0.0f)
//						model = glm::translate(model, glm::vec3(mainWindow.getrotax() * 0.1, 0.0f, 0.0f));
				    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
					color = (rotate == 0.0f) ? ColorFromRGB(255, 255, 255) : ColorFromRGB(123, 123, 123);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					meshMap["Triangle"]->RenderMesh();
				    rotate = (rotate == 0.0f) ? 180.0f : 0.0f;
				}
//			}
		}

//		model = glm::mat4(1.0);
//		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
//		model = glm::scale(model, glm::vec3(4.0f, 1.0f, 4.0f));
//		color = ColorFromRGB(220, 220, 220);
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//		meshMap["Plane"]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

#pragma clang diagnostic pop