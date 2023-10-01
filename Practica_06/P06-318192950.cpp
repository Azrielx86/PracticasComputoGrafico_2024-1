#pragma warning(push, 0)
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "readability-non-const-parameter"
#pragma ide diagnostic ignored "UnusedLocalVariable"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma ide diagnostic ignored "modernize-use-emplace"
/*
Semestre 2024-1
Practica 6: Texturizado
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#define ColorRGB(r, g, b)                  \
	{                                      \
		r / 255.0f, g / 255.0f, b / 255.0f \
	}

#include <glew.h>
#include <glfw3.h>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <unordered_map>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelMatrix.h"
#include "Shader_m.h"
#include "Skybox.h"
#include "Texture.h"
#include "Window.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

// No muy útil para esta práctica, pero tal vez en siguientes si
enum
{
	DADO_8F
};
// unordered_map tiene mejor rendimiento que un map
std::unordered_map<int, Mesh *> meshUoMap;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture dado8fTexture;
Texture wheelTexture;

Model Dado_M, Dado8f;
Model car, hood, wheel;

Skybox skybox;

// Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char *vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char *fShader = "shaders/shader_texture.frag";

// c?lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1],
		             vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1],
		             vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
	    0, 3, 1,
	    1, 3, 2,
	    2, 3, 0,
	    0, 1, 2};
	// clang-format off
    GLfloat vertices[] = {
            //	x      y      z			u	  v			nx	  ny    nz
            -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
            0, 2, 1,
            1, 2, 3
    };

    GLfloat floorVertices[] = {
            -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };

    unsigned int vegetacionIndices[] = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7
    };

    GLfloat vegetacionVertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

            0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
	// clang-format on
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
}

void CreateShaders()
{
	auto shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDadoOchoCaras()
{
	// clang-format off
    unsigned int dado_idx[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23
    };
    //Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
    GLfloat dado_vtx[] = {
            // Cara superior 1
            //x		y		z		S		T			NX		NY		NZ
            0.0f, 1.0f, 0.0f, 0.5, 0.75, 1.0f, 1.0f, 1.0f, // 0
            0.0f, 0.0f, -1.0f, 0.75, 1.0, 1.0f, 1.0f, 1.0f, //1
            1.0f, 0.0f, 0.0f, 1.0f, 0.75, 1.0f, 1.0f, 1.0f, // 2

            // Cara superior 3
            0.0f, 1.0f, 0.0f, 0.5, 0.75, 1.0f, 1.0f, 1.0f, // 3
            1.0f, 0.0f, 0.0f, 1.0f, 0.75, 1.0f, 1.0f, 1.0f, // 4
            0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 5

            // Cara superior 5
            0.0f, 1.0f, 0.0f, 0.5f, 0.75, 1.0f, 1.0f, 1.0f, // 6
            0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 7
            -1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 8

            // Cara superior 7
            0.0f, 1.0f, 0.0f, 0.5f, 0.75, 1.0f, 1.0f, 1.0f, // 9 
            -1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 10
            0.0f, -0.0f, -1.0f, 0.0f, 0.75f, 1.0f, 1.0f, 1.0f, // 11,

            // Caras inferiores
            // Cara inferior 4
            0.0f, -1.0f, 0.0f, 0.5, 0.25, 1.0f, 1.0f, 1.0f, // 12
            0.0f, 0.0f, -1.0f, 0.75, 0.0, 1.0f, 1.0f, 1.0f, //13
            1.0f, 0.0f, 0.0f, 1.0f, 0.25, 1.0f, 1.0f, 1.0f, // 14

            // Cara superior 6
            0.0f, -1.0f, 0.0f, 0.5, 0.25, 1.0f, 1.0f, 1.0f, // 15
            1.0f, 0.0f, 0.0f, 1.0f, 0.25, 1.0f, 1.0f, 1.0f, // 16
            0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 17

            // Cara superior 8
            0.0f, -1.0f, 0.0f, 0.5f, 0.25, 1.0f, 1.0f, 1.0f, // 18
            0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 19
            -1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 20

            // Cara superior 2
            0.0f, -1.0f, 0.0f, 0.5f, 0.25, 1.0f, 1.0f, 1.0f, // 21
            -1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 22
            0.0f, -0.0f, -1.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f, // 23
    };

	// clang-format on
	Mesh *dado = new Mesh();
	dado->CreateMesh(dado_vtx, dado_idx, 192, 36);
	meshUoMap[DADO_8F] = dado;
}

void CrearDado()
{
	// clang-format off
    unsigned int cubo_indices[] = {
            // front
            0, 1, 2,
            2, 3, 0,

            // back
            8, 9, 10,
            10, 11, 8,

            // left
            12, 13, 14,
            14, 15, 12,
            // bottom
            16, 17, 18,
            18, 19, 16,
            // top
            20, 21, 22,
            22, 23, 20,

            // right
            4, 5, 6,
            6, 7, 4,

    };
    //Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
    GLfloat cubo_vertices[] = {
            // front
            //x		y		z		S		T			NX		NY		NZ
            -0.5f, -0.5f, 0.5f, 0.26f, 0.34f, 0.0f, 0.0f, -1.0f,    //0
            0.5f, -0.5f, 0.5f, 0.49f, 0.34f, 0.0f, 0.0f, -1.0f,    //1
            0.5f, 0.5f, 0.5f, 0.49f, 0.66f, 0.0f, 0.0f, -1.0f,    //2
            -0.5f, 0.5f, 0.5f, 0.26f, 0.66f, 0.0f, 0.0f, -1.0f,    //3
            // right
            //x		y		z		S		T
            0.5f, -0.5f, 0.5f, 0.0f, 0.33f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.25f, 0.33f, -1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.25f, 0.66f, -1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.66f, -1.0f, 0.0f, 0.0f,
            // back
            -0.5f, -0.5f, -0.5f, 0.5f, 0.33f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.75f, 0.33f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.75f, 0.66f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.5f, 0.66f, 0.0f, 0.0f, 1.0f,

            // left
            //x		y		z		S		T
            -0.5f, -0.5f, -0.5f, 0.75f, 0.33f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.33f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.66f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.75f, 0.66f, 1.0f, 0.0f, 0.0f,

            // bottom
            //x		y		z		S		T
            -0.5f, -0.5f, 0.5f, 0.50f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.75f, 0.33f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.50f, 0.33f, 0.0f, 1.0f, 0.0f,

            //UP
            //x		y		z		S		T
            -0.5f, 0.5f, 0.5f, 0.50f, 0.66f, 0.0f, -1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.75f, 0.66f, 0.0f, -1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.75f, 1.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.50f, 1.0f, 0.0f, -1.0f, 0.0f,

    };
	// clang-format on
	Mesh *dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	ModelMatrix handler(glm::mat4(1.0f));

	CreateObjects();
	CrearDado();
	CrearDadoOchoCaras();
	CreateShaders();
#ifdef __linux__
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.3f, 1.5f);
#else
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);
#endif

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_anim_edit.tga");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	dado8fTexture = Texture("Textures/dado_8f.tga");
	dado8fTexture.LoadTextureA();
	wheelTexture = Texture("Textures/BMW/WheelSingleTexture.tga");
	wheelTexture.LoadTextureA();

	car = Model();
	car.LoadModel("Models/BMW_no_hood.obj");

	hood = Model();
	hood.LoadModel("Models/BMW_Hood_logo.obj");

	wheel = Model();
	wheel.LoadModel("Models/BMW_Wheel.obj");

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
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
	                                        0.1f, 1000.0f);

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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y,
		            camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		/*Reporte de pr?ctica :
		Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de c?digo
		 */
		model = handler.setMatrix(glm::mat4(1.0f))
		            .addTranslation(5.0f, 5.0, 5.0)
		            .addScale(2.0f, 2.0f, 2.0f)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // TODO : Meter esta línea en ModelMatrix.cpp
		dado8fTexture.UseTexture();
		meshUoMap.at(DADO_8F)->RenderMesh();
		
		/*
		Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y
		texturizar el logo de la Facultad de ingenier?a en el cofre de su propio modelo de coche
		*/

		//------------ INICIA DIBUJO DEL VEHICULO -------------------
		// Vehiculo
		Texture::DisableTextures();
		model = glm::mat4(1.0);
		model = handler.setMatrix(model)
		            .addTranslation(mainWindow.getPosVehiculo(), 0.2f, 0.0f)
		            .saveActualState(modelaux) // modelaux = model;
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		car.RenderModel();

		// Cofre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux) // model = modelaux;
		            .addTranslation(1.37413, 1.57, 0)
		            .addRotationZ(mainWindow.getAnguloCofre())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logofiTexture.UseTexture();
		hood.RenderModel();

		wheelTexture.UseTexture();
		// Rueda derecha trasera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(-2.92984, 0.52, 1.28414)
		            .addRotationZ(-mainWindow.getRotaRuedas())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda izquierda trasera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(-2.92984, 0.52, -1.28414)
		            .addRotationZ(-mainWindow.getRotaRuedas())
		            .addRotationX(-180)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda derecha delantera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(2.6089, 0.52, 1.28414)
		            .addRotationZ(-mainWindow.getRotaRuedas())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda izquierda delantera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = handler.setMatrix(modelaux)
		            .addTranslation(2.6089, 0.52, -1.28414)
		            .addRotationZ(-mainWindow.getRotaRuedas())
		            .addRotationX(-180)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        logofiTexture.UseTexture(); //textura con transparencia o traslucidez
        FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
        glDisable(GL_BLEND);
*/
#pragma clang diagnostic pop
#pragma warning(pop)