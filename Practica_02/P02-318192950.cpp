#ifdef CMAKE_PROJECT
// Para compilar con cmake, utilizando los paquetes de OpenGL, GLFW y glew
// instalados en Linux, o con vcpkg en Windows.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#else
// Para Visual Studio sin vcpkg.
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#endif
#include <map>

// Comentar o descomentar para cambiar entre los
// objetos que se van a renderizar.
// O definirlo en CMake.
// #define LETRAS

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

Window mainWindow;

// Una coleccion cadena-mesh, con la cual se podra acceder al objeto mesh
// desde un nombre, en lugar de indices.
// De igual manera se agregaron meshColorMap y shaderMap.
std::map<std::string, MeshColor *> meshColorMap;
std::map<std::string, Mesh *> meshMap;
std::map<std::string, Shader> shaderMap;

// Vertex Shader
static const char *vShader = "shaders/shader.vert";
static const char *fShader = "shaders/shader.frag";
static const char *vShaderColor = "shaders/shadercolor.vert";
static const char *fShaderColor = "shaders/shadercolor.frag";

// shaders nuevos se crearan aqui
static const char *vShaderRojo = "shaders/shader_rojo.vert";
static const char *vShaderAzul = "shaders/shader_azul.vert";
static const char *vShaderVerde = "shaders/shader_verde.vert";
static const char *vShaderCafe = "shaders/shader_cafe.vert";
static const char *vShaderVerdeObscuro = "shaders/shader_verde_obscuro.vert";

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
	meshMap["Piramide"] = obj1;
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
	meshMap["Cubo"] = cubo;
}

void CrearLetrasyFiguras()
{
	// clang-format off
	GLfloat letra_e_array[] = {	
		//X     Y     Z     R     G     B
		-8.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-8.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-8.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 1.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -1.5f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 1.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -1.5f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 1.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -1.5f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
	};

	GLfloat letra_u_array[] = {
		-4.0f, 6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, 6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-4.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, 6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-4.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-4.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, -6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, 6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-1.0f, 6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-1.0f, 6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-1.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-1.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, -6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-3.0f, -6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.4f,
		-2.0f, -6.0f, 0.0f, 0.0f, 0.3f, 0.4f,
	};

    GLfloat letra_m_array[] = {
            1.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            1.0f, -6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            1.0f, -6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.0f, -6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            3.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            4.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            3.0f, -6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            4.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            3.0f, -6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            4.0f, -6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.0f, 4.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.5f, 3.5f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.5f, 3.5f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.5f, 5.5f, 0.0f, 0.9f, 0.2f, 0.2f,
            3.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.5f, 3.5f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.5f, 5.5f, 0.0f, 0.9f, 0.2f, 0.2f,
            3.0f, 6.0f, 0.0f, 0.9f, 0.2f, 0.2f,
            2.5f, 3.5f, 0.0f, 0.9f, 0.2f, 0.2f,
            3.0f, 4.0f, 0.0f, 0.9f, 0.2f, 0.2f,
    };

	GLfloat letra_c_array[] = {
		6.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		6.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		6.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, 4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, -4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, -4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
	};

	// clang-format on
	auto letra_e = new MeshColor();
	letra_e->CreateMeshColor(letra_e_array, 144);
	meshColorMap["LetraE"] = letra_e;

	auto letra_u = new MeshColor();
	letra_u->CreateMeshColor(letra_u_array, 144);
	meshColorMap["LetraU"] = letra_u;

	auto letra_m = new MeshColor();
	letra_m->CreateMeshColor(letra_m_array, 144);
	meshColorMap["LetraM"] = letra_m;

	auto letra_c = new MeshColor();
	letra_c->CreateMeshColor(letra_c_array, 108);
	meshColorMap["LetraC"] = letra_c;
}

void CreateShaders()
{
	auto shader1 = new Shader(); // shader para usar ndices: objetos: cubo y  pirmide
	shader1->CreateFromFiles(vShader, fShader);
	shaderMap["VertexShader"] = *shader1;

	auto shader2 = new Shader(); // shader para usar color como parte del VAO: letras
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderMap["VertexColor"] = *shader2;

	auto shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShaderColor);
	shaderMap["ShaderRojo"] = *shaderRojo;

	auto shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShaderAzul, fShaderColor);
	shaderMap["ShaderAzul"] = *shaderAzul;

	auto shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShaderVerde, fShaderColor);
	shaderMap["ShaderVerde"] = *shaderVerde;

	auto shaderVerdeObscuro = new Shader();
	shaderVerdeObscuro->CreateFromFiles(vShaderVerdeObscuro, fShaderColor);
	shaderMap["ShaderVerdeObscuro"] = *shaderVerdeObscuro;

	auto shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShaderCafe, fShaderColor);
	shaderMap["ShaderCafe"] = *shaderCafe;
}

/**
 * Función para utilizar un shader desde el shaderMap
 */
void UseShader(const char *shaderName, GLuint &uniformModel, GLuint &uniformProjection)
{
	shaderMap[shaderName].useShader();
	uniformModel = shaderMap[shaderName].getModelLocation();
	uniformProjection = shaderMap[shaderName].getProjectLocation();
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide();        // indice 0 en MeshList
	CrearCubo();           // indice 1 en MeshList
	CrearLetrasyFiguras(); // usa MeshColor, ndices en MeshColorList
	CreateShaders();
	GLuint uniformProjection;
	GLuint uniformModel;
	//	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		glfwPollEvents();

#ifdef LETRAS
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Se agrega limpiar el buffer de profundidad
		// Para las letras hay que usar el segundo set de shaders con ndice 1 en ShaderList
		shaderMap["VertexColor"].useShader();
		uniformModel = shaderMap["VertexColor"].getModelLocation();
		uniformProjection = shaderMap["VertexColor"].getProjectLocation();

		// Inicializar matriz de dimensin 4x4 que servir como matriz de modelo para almacenar las transformaciones geomtricas
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["LetraE"]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["LetraU"]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["LetraM"]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorMap["LetraC"]->RenderMeshColor();
#else
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UseShader("ShaderRojo", uniformModel, uniformProjection);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.15f, -5.0f));
		model = glm::scale(model, glm::vec3(0.65f, 0.85f, 0.65f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Cubo"]->RenderMesh();

		// Tejado
		UseShader("ShaderAzul", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.52f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Piramide"]->RenderMesh();

		// Puerta central
		UseShader("ShaderVerde", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, -4.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.35f, 0.25f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Cubo"]->RenderMesh();

		// Ventana izquierda
		UseShader("ShaderVerde", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.17f, 0.05f, -4.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Cubo"]->RenderMesh();

		// Ventana derecha
		UseShader("ShaderVerde", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.17f, 0.05f, -4.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Cubo"]->RenderMesh();

		// Pino izquierdo
		UseShader("ShaderCafe", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.55f, -5.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Cubo"]->RenderMesh();

		UseShader("ShaderVerdeObscuro", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.8f, 0.45f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Piramide"]->RenderMesh();

		// Pino Derecho
		UseShader("ShaderCafe", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, -0.55f, -5.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Cubo"]->RenderMesh();

		UseShader("ShaderVerdeObscuro", uniformModel, uniformProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.8f, 0.45f));
		glUniformMatrix4fv((GLint) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshMap["Piramide"]->RenderMesh();
#endif
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
