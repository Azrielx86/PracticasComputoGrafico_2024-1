#pragma warning(push, 0)
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedLocalVariable"
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "modernize-use-emplace"
#pragma ide diagnostic ignored "modernize-deprecated-headers"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
/*
Semestre 2024-1
Pr?ctica 5: Optimizaci?n y Carga de Modelos
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#define ColorRGB(r, g, b)                  \
	{                                      \
		r / 255.0f, g / 255.0f, b / 255.0f \
	}

#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelMatrix.h"
#include "Shader_m.h"
#include "Skybox.h"
#include "Window.h"

// float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model car, hood, wheel;

Skybox skybox;

// Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char *vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char *fShader = "shaders/shader_m.frag";

void CreateObjects()
{
	unsigned int indices[] = {
	    0, 3, 1,
	    1, 3, 2,
	    2, 3, 0,
	    0, 1, 2};

	GLfloat vertices[] = {
	    //	x      y      z			u	  v			nx	  ny    nz
	    -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
	    1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f};

	unsigned int floorIndices[] = {
	    0, 2, 1,
	    1, 2, 3};

	GLfloat floorVertices[] = {
	    -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	    10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	    -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
	    10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	auto shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// glm::vec3 ColorRGB(int r, int g, int b) { return {r / 255.0f, g / 255.0f, b / 255.0f}; }

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	car = Model();
	car.LoadModel("Models/BMW_no_hood.obj");

	hood = Model();
	hood.LoadModel("Models/BMW_Hood.obj");

	wheel = Model();
	wheel.LoadModel("Models/BMW_Wheel.obj");

	ModelMatrix handler(glm::mat4(1.0f));

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
	       uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); // piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------ INICIA DIBUJO DEL VEHICULO -------------------
		// Vehiculo
		color = ColorRGB(189, 189, 189);
		model = glm::mat4(1.0);
		model = handler.setMatrix(model)
		            .addTranslation(mainWindow.getPosVehiculo(), 0.2f, 0.0f)
		            .saveActualState(modelaux) // modelaux = model;
		            .getMatrix();
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		car.RenderModel();

		// Cofre
		color = ColorRGB(61, 82, 115);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux) // model = modelaux;
		            .addTranslation(1.64, 1.53, 0)
		            .addRotationZ(mainWindow.getAnguloCofre())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hood.RenderModel();

		// Rueda derecha trasera
		color = ColorRGB(0, 0, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(-2.8, 0.52, 1.74)
		            .addRotationZ(mainWindow.getRotaRuedas())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda izquierda trasera
		color = ColorRGB(0, 0, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(-2.8, 0.52, -1.74)
		            .addRotationZ(mainWindow.getRotaRuedas())
		            .addRotationX(-180)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda derecha delantera
		color = ColorRGB(0, 0, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(3.11, 0.52, 1.74)
		            .addRotationZ(mainWindow.getRotaRuedas())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda izquierda delantera
		color = ColorRGB(0, 0, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(3.11, 0.52, -1.74)
		            .addRotationZ(mainWindow.getRotaRuedas())
		            .addRotationX(-180)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

#pragma clang diagnostic pop
#pragma warning(pop)
