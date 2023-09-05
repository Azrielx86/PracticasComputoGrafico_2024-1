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

void CrearCubo()
{
	// clang-format off
	unsigned int cubo_indices[] = {
	    // front
	    0, 1, 2,
	    2, 3, 0,
	    // right
	    1, 5, 6,
	    6, 2, 1,
	    // back
	    7, 6, 5,
	    5, 4, 7,
	    // left
	    4, 0, 3,
	    3, 7, 4,
	    // bottom
	    4, 5, 1,
	    1, 0, 4,
	    // top
	    3, 2, 6,
	    6, 7, 3};

	GLfloat cubo_vertices[] = {
	    // front
	    -0.5f, -0.5f, 0.5f,
	    0.5f, -0.5f, 0.5f,
	    0.5f, 0.5f, 0.5f,
	    -0.5f, 0.5f, 0.5f,
	    // back
	    -0.5f, -0.5f, -0.5f,
	    0.5f, -0.5f, -0.5f,
	    0.5f, 0.5f, -0.5f,
	    -0.5f, 0.5f, -0.5f};
	// clang-format on
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
	meshMap["Cubo"] = cubo;
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	// clang-format off
	unsigned int indices_piramide_triangular[] = {
	    0, 1, 2,
	    1, 3, 2,
	    3, 0, 2,
	    1, 0, 3

	};
	GLfloat vertices_piramide_triangular[] = {
	    -0.5f, -0.5f, 0.0f, // 0
	    0.5f, -0.5f, 0.0f,  // 1
	    0.0f, 0.5f, -0.25f, // 2
	    0.0f, -0.5f, -0.5f, // 3

	};
	// clang-format on
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}
/**
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R)
{

	// constantes utilizadas en los ciclos for
	int n, i;
	// cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	// ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++)
	{
		if (n != res)
		{
			x = R * cos((n) *dt);
			z = R * sin((n) *dt);
		}
		// caso para terminar el círculo
		else
		{
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	// ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++)
	{
		x = R * cos((n) *dt);
		z = R * sin((n) *dt);
		for (i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	// ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++)
	{
		x = R * cos((n) *dt);
		z = R * sin((n) *dt);
		for (i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	// Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++)
		indices.push_back(i);

	// se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

// función para crear un cono
void CrearCono(int res, float R)
{

	// constantes utilizadas en los ciclos for
	int n, i;
	// cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	// caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	// ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++)
	{
		x = R * cos((n) *dt);
		z = R * sin((n) *dt);
		for (i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);

	for (i = 0; i < res + 2; i++)
		indices.push_back(i);

	// se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

// función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	// clang-format off
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4
	
	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	// clang-format on
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
	meshMap["PiramideCuadrada"] = piramide;
}

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
	if (r > 255 || g > 255 || b > 255)
		throw std::exception();
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

	CrearCubo();                 // índice 0 en MeshList
	CrearPiramideTriangular();   // índice 1 en MeshList
	CrearCilindro(5, 1.0f);      // índice 2 en MeshList
	CrearCono(25, 2.0f);         // índice 3 en MeshList
	CrearPiramideCuadrangular(); // índice 4 en MeshList
	CreateShaders();
	CreatePlane();

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

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

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(-2.0f, 1.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = ColorFromRGB(220, 220, 220);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshMap["Plane"]->RenderMesh();

		// casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = ColorFromRGB(220, 20, 60);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshMap["Cubo"]->RenderMesh();


		// techo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 1.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = ColorFromRGB(65, 105, 225);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshMap["PiramideCuadrada"]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

#pragma clang diagnostic pop