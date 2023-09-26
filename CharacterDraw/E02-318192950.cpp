#ifdef CMAKE_PROJECT
// Para compilar con cmake, utilizando los paquetes de OpenGL, GLFW y glew
// instalados en Linux, o con vcpkg en Windows.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <glew.h>
#include <glfw3.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.14159265f / 180.0; // grados a radianes
Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

// Una coleccion cadena-mesh, con la cual se podra acceder al objeto mesh
// desde un nombre, en lugar de indices.
std::map<std::string, MeshColor *> meshColorMap;

// Vertex Shader
static const char *vShader = "shaders/shader.vert";
static const char *fShader = "shaders/shader.frag";
static const char *vShaderColor = "shaders/shadercolor.vert";
static const char *fShaderColor = "shaders/shadercolor.frag";
// shaders nuevos se crearan aqui

float angulo = 0.0f;

// color cafe: 0.478, 0.255, 0.067

// Piramide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
	    0, 1, 2,
	    1, 3, 2,
	    3, 0, 2,
	    1, 0, 3};

	GLfloat vertices[] = {
	    -0.5f, -0.5f, 0.0f, // 0
	    0.5f, -0.5f, 0.0f,  // 1
	    0.0f, 0.5f, -0.25f, // 2
	    0.0f, -0.5f, -0.5f, // 3
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

// Vrtices de un cubo
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
}

void CrearLetrasyFiguras()
{
	// clang-format off
	// ===== TRIANGULO AZUL =====
    GLfloat vertices_trianguloAzul[] = {
            //X			Y			Z			R		G		B
            -1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    MeshColor *TrianguloAzul = new MeshColor();
    TrianguloAzul->CreateMeshColor(vertices_trianguloAzul, 18);
    meshColorMap["TrianguloAzul"] = TrianguloAzul;

	// ===== TRIANGULO ROJO =====
    GLfloat vertices_triangulorojo[] = {
            //X			Y			Z			R		G		B
            -1.0f, -1.0f, 0.5f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f,
    };

    MeshColor *triangulorojo = new MeshColor();
    triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
    meshColorMap["TrianguloRojo"] = triangulorojo;

	// ===== TRIANGULO VERDE =====
    GLfloat vertices_trianguloVerde[] = {
            //X			Y			Z			R		G		B
            -1.0f, -1.0f, 0.5f, 0.0f, 0.5f, 0.0f,
            1.0f, -1.0f, 0.5f, 0.0f, 0.5f, 0.0f,
            0.0f, 1.0f, 0.5f, 0.0f, 0.5f, 0.0f,
    };

    MeshColor *trianguloVerde = new MeshColor();
    trianguloVerde->CreateMeshColor(vertices_trianguloVerde, 18);
    meshColorMap["TrianguloVerde"] = trianguloVerde;

	// ===== CUADRADO VERDE =====
    GLfloat vertices_cuadradoverde[] = {
            //X			Y			Z			R		G		B
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

    };

    MeshColor *cuadradoverde = new MeshColor();
    cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
    meshColorMap["CuadradoVerde"] = cuadradoverde;

	// ===== CUADRADO ROJO =====
    GLfloat vertices_cuadradorojo[] = {
            //X			Y			Z			R		G		B
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    };

    MeshColor *cuadradoRojo = new MeshColor();
    cuadradoRojo->CreateMeshColor(vertices_cuadradorojo, 36);
    meshColorMap["CuadradoRojo"] = cuadradoRojo;
	
	// ===== CUADRADO CAFE =====
    GLfloat vertices_cuadradoCafe[] = {
            //X			Y			Z			R		G		B
            -0.5f, -0.5f, 0.5f,  0.478, 0.255, 0.067,
            0.5f, -0.5f, 0.5f, 0.478, 0.255, 0.067,
            0.5f, 0.5f, 0.5f, 0.478, 0.255, 0.067,
            -0.5f, -0.5f, 0.5f, 0.478, 0.255, 0.067,
            0.5f, 0.5f, 0.5f, 0.478, 0.255, 0.067,
            -0.5f, 0.5f, 0.5f, 0.478, 0.255, 0.067,

    };

    MeshColor *cuadradoCafe = new MeshColor();
    cuadradoCafe->CreateMeshColor(vertices_cuadradoCafe, 36);
    meshColorMap["CuadradoCafe"] = cuadradoCafe;
	// clang-format on
}

void CreateShaders()
{
	Shader *shader1 = new Shader(); // shader para usar ndices: objetos: cubo y  pirmide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader(); // shader para usar color como parte del VAO: letras
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide();        // indice 0 en MeshList
	CrearCubo();           // indice 1 en MeshList
	CrearLetrasyFiguras(); // usa MeshColor, ndices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		// Recibir eventos del usuario
		glfwPollEvents();
		// Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Se agrega limpiar el buffer de profundidad

		// Para las letras hay que usar el segundo set de shaders con ndice 1 en ShaderList
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		// Inicializar matriz de dimensin 4x4 que servir como matriz de modelo para almacenar las transformaciones geomtricas
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, -4.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["TrianguloAzul"]->RenderMeshColor();

		// Base de la casa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, -5.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["CuadradoRojo"]->RenderMeshColor();

		// Cuadrado verde izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.08f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["CuadradoVerde"]->RenderMeshColor();

		// Cuadrado verde derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 0.08f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["CuadradoVerde"]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["CuadradoVerde"]->RenderMeshColor();

		// Pino izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["CuadradoCafe"]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.15f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.45f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["TrianguloVerde"]->RenderMeshColor();

		// Pino derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, -0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["CuadradoCafe"]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, -0.15f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.45f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["TrianguloVerde"]->RenderMeshColor();
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}