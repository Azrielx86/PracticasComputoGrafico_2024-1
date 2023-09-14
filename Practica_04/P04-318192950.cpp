#pragma warning(push, 0)
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
// semestre 2024-1 pr�ctica 4: Modelado Jer�rquico.
// el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere
// heredar entre diversas instancias para que est�n unidas
#include <cmath>
#include <glew.h>
#include <glfw3.h>
#include <vector>
// glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// clases para dar orden y limpieza al c�digo
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"

enum Figures
{
	FIG_CUBO,
	FIG_PIRAMIDE_TRIANG,
	FIG_CILINDRO,
	FIG_CONO,
	FIG_PIRAMIDE_CUAD
};

using std::vector;

// grados a radianes
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
Sphere sp = Sphere(1.0, 20, 20); // recibe radio, slices, stacks

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
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	// clang-format on
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	// clang-format off
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	// clang-format on
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}
/*
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R)
{

	// constantes utilizadas en los ciclos for
	int n, i;
	// c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	// ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++)
	{
		if (n != res)
		{
			x = R * cos((n) *dt);
			z = R * sin((n) *dt);
		}
		// caso para terminar el c�rculo
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

	// Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++)
		indices.push_back(i);

	// se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

// funci�n para crear un cono
void CrearCono(int res, float R)
{

	// constantes utilizadas en los ciclos for
	int n, i;
	// c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	// caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	// ciclo for para crear los v�rtices de la circunferencia del cono
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

// funci�n para crear pir�mide cuadrangular unitaria
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
}

void CreateShaders()
{
	auto *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

glm::vec3 ColorRGB(int r, int g, int b) { return {r / 255.0f, g / 255.0f, b / 255.0f}; }

int main()
{
	//	mainWindow = Window(800, 600);
	mainWindow = Window(1280, 720);
	mainWindow.Initialise();
	// Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();                 // �ndice 0 en MeshList
	CrearPiramideTriangular();   // �ndice 1 en MeshList
	CrearCilindro(25, 1.0f);     // �ndice 2 en MeshList
	CrearCono(25, 2.0f);         // �ndice 3 en MeshList
	CrearPiramideCuadrangular(); // �ndice 4 en MeshList
	CreateShaders();

	camera = Camera(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	// glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	// Loop mientras no se cierra la ventana
	sp.init(); // inicializar esfera
	sp.load(); // enviar la esfera al shader

	glm::mat4 model(1.0);    // Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0); // Inicializar matriz de Modelo 4x4
	glm::mat4 modelBaseAux(1.0f);

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
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// Eje z
		model = glm::mat4(1.0f);
		model = glm::translate(model, {0.0f, 0.0f, 50.0f});
		model = glm::scale(model, {0.1f, 0.1f, 100.0f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(0, 0, 255);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		// Eje x
		model = glm::mat4(1.0f);
		model = glm::translate(model, {50.0f, 0.0f, 0.0f});
		model = glm::scale(model, {100.0f, 0.1f, 0.1f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(255, 0, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		// Eje z
		model = glm::mat4(1.0f);
		model = glm::translate(model, {0.0f, 50.0f, 0.0f});
		model = glm::scale(model, {0.1f, 100.0f, 0.1f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(0, 255, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		// Cuerpo robot
		model = glm::mat4(1.0f);
		model = glm::translate(model, {0.0f, 5.0f, -4.0f});
		modelBaseAux = modelaux = model;
		model = glm::scale(model, {3.8f, 1.8f, 2.2f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(220, 20, 60);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		// Pie derecho (?)
		model = modelBaseAux;
		model = glm::translate(model, {-1.2f, -0.4f, -1.2});
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), {0.0f, 0.0f, 1.0f});
		modelaux = model;
		model = glm::scale(model, {0.5f, 0.5f, 0.5f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(20, 220, 60);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;
		model = glm::translate(model, {0.0f, -0.8f, 0.0});
		modelaux = model;
		model = glm::scale(model, {0.8f, 1.6f, 0.8f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		model = modelaux;
		model = glm::translate(model, {0.0f, -0.8f, 0.0f});
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), {0.0f, 0.0f, 1.0f});
		modelaux = model;
		model = glm::scale(model, {0.4f, 0.4f, 0.4f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(20, 60, 220);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;
		model = glm::translate(model, {0.0f, -0.8f, 0.0});
		modelaux = model;
		model = glm::scale(model, {0.8f, 1.6f, 0.8f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		// Pie izquierdo trasero
		model = modelBaseAux;
		model = glm::translate(model, {-1.2f, -0.4f, 1.2});
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), {0.0f, 0.0f, 1.0f});
		modelaux = model;
		model = glm::scale(model, {0.5f, 0.5f, 0.5f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(20, 220, 60);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;
		model = glm::translate(model, {0.0f, -0.8f, 0.0});
		modelaux = model;
		model = glm::scale(model, {0.8f, 1.6f, 0.8f});
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = ColorRGB(20, 220, 60);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[FIG_CUBO]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

#pragma clang diagnostic pop
#pragma warning(pop)