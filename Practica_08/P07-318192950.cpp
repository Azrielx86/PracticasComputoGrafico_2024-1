#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Weverything"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "modernize-use-emplace"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
/*
Semestre 2024-1
Pr?ctica 7: Iluminaci?n 1
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <unordered_map>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader_light.h"
#include "Skybox.h"
#include "Texture.h"
#include "Window.h"

// para iluminaci?n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "LightCollection.h"
#include "Material.h"
#include "ModelMatrix.h"
#include "PointLight.h"
#include "SpotLight.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

enum
{
	DADO_8F
};
std::unordered_map<int, Mesh *> meshUnorderedMap;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dado8fTexture;
Texture wheelTexture;
Texture logofiTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Faro_M;
Model car, hood, wheel;

Skybox skybox;

// materiales
Material Material_brillante;
Material Material_opaco;

// Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
// para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char *vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char *fShader = "shaders/shader_light.frag";

// funci?n de calculo de normales por promedio de v?rtices
void calcAverageNormals(const unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
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
	// clang-format off
	unsigned int indices[] = {
	    0, 3, 1,
	    1, 3, 2,
	    2, 3, 0,
	    0, 1, 2
	};
	
	GLfloat vertices[] = {
	    //	x      y      z			u	  v			nx	  ny    nz
	    -1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	    0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
	    1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
	    0, 2, 1,
	    1, 2, 3
	};

	GLfloat floorVertices[] = {
	    -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	    10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	    -10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
	    10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	    0, 1, 2,
	    0, 2, 3,
	    4,5,6,
	    4,6,7
	};

	GLfloat vegetacionVertices[] = {
	    -0.5f, -0.5f, 0.0f,		     0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
	    0.5f, -0.5f, 0.0f,		    1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    -0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

	    0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	};
	// clang-format on

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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
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
		// Cara superior 4
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 1.0f, 0.0f, 0.5, 0.75, -1.0f, -1.0f, 1.0f, // 0
		0.0f, 0.0f, -1.0f, 0.75, 1.0, -1.0f, -1.0f, 1.0f, //1
		1.0f, 0.0f, 0.0f, 1.0f, 0.75, -1.0f, -1.0f, 1.0f, // 2

		// Cara superior 6
		0.0f, 1.0f, 0.0f, 0.5, 0.75, -1.0f, -1.0f, -1.0f, // 3
		1.0f, 0.0f, 0.0f, 1.0f, 0.75, -1.0f, -1.0f, -1.0f, // 4
		0.0f, 0.0f, 1.0f, 0.75, 0.50, -1.0f, -1.0f, -1.0f, // 5

		// Cara superior 8
		0.0f, 1.0f, 0.0f, 0.5f, 0.75, 1.0f, -1.0f, -1.0f, // 6
		0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, -1.0f, -1.0f, // 7
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, -1.0f, -1.0f, // 8

		// Cara superior 2
		0.0f, 1.0f, 0.0f, 0.5f, 0.75, 1.0f, -1.0f, 1.0f, // 9 
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, -1.0f, 1.0f, // 10
		0.0f, -0.0f, -1.0f, 0.0f, 0.75f, 1.0f, -1.0f, 1.0f, // 11,

		// Caras inferiores
		// Cara inferior 1
		0.0f, -1.0f, 0.0f, 0.5, 0.25, -1.0f, 1.0f, 1.0f, // 12
		0.0f, 0.0f, -1.0f, 0.75, 0.0, -1.0f, 1.0f, 1.0f, //13
		1.0f, 0.0f, 0.0f, 1.0f, 0.25, -1.0f, 1.0f, 1.0f, // 14

		// Cara superior 3
		0.0f, -1.0f, 0.0f, 0.5, 0.25, -1.0f, 1.0f, -1.0f, // 15
		1.0f, 0.0f, 0.0f, 1.0f, 0.25, -1.0f, 1.0f, -1.0f, // 16
		0.0f, 0.0f, 1.0f, 0.75, 0.50, -1.0f, 1.0f, -1.0f, // 17

		// Cara superior 5
		0.0f, -1.0f, 0.0f, 0.5f, 0.25, 1.0f, 1.0f, -1.0f, // 18
		0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, -1.0f, // 19
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, -1.0f, // 20

		// Cara superior 7
		0.0f, -1.0f, 0.0f, 0.5f, 0.25, 1.0f, 1.0f, 1.0f, // 21
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 22
		0.0f, -0.0f, -1.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f, // 23
	};

	// clang-format on
	Mesh *dado = new Mesh();
	dado->CreateMesh(dado_vtx, dado_idx, 192, 36);
	calcAverageNormals(dado_idx, 24, dado_vtx, 192, 8, 5);
	meshUnorderedMap[DADO_8F] = dado;
}

int main()
{
	mainWindow = Window(1280, 720); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	ModelMatrix handler(glm::mat4(1.0f));

	CreateObjects();
	CreateShaders();
	CrearDadoOchoCaras();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	dado8fTexture = Texture("Textures/dado_8f.tga");
	dado8fTexture.LoadTextureA();
	wheelTexture = Texture("Textures/BMW/WheelSingleTexture.tga");
	wheelTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Faro_M = Model();
	Faro_M.LoadModel("Models/faro.obj");

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

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// luz direccional, solo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
	                             0.3f, 0.3f,
	                             0.0f, 0.0f, -1.0f);
	// contador de luces puntuales
	unsigned int pointLightCount = 0;
	// Declaraci?n de primer luz puntual
	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
	                            0.0f, 1.0f,
	                            -6.0f, 1.5f, -5.5f,
	                            0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
	                            0.0f, 1.0f,
	                            -6.0f, 1.5f, -5.5f,
	                            0.3f, 0.01f, 0.001f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	// linternaCamara
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	                          0.0f, 2.0f,
	                          0.0f, 0.0f, 0.0f,
	                          0.0f, -1.0f, 0.0f,
	                          1.0f, 0.0f, 0.0f,
	                          15.0f);
	spotLightCount++;

	// luz fija
	spotLights[1] = SpotLight(0.6f, 0.0f, 0.8f,
	                          1.0f, 2.0f,
	                          0.0f, 4.0f, -14.0f,
	                          0.0f, -5.0f, 0.0f,
	                          1.0f, 0.0f, 0.0f,
	                          15.0f);
	spotLightCount++;

	// se crean mas luces puntuales y spotlight
	// Luz helicoptero
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
	                          1.0f, 2.0f,
	                          5.0f, 10.0f, 0.0f,
	                          0.0f, -5.0f, 0.0f,
	                          1.0f, 0.01f, 0.01f,
	                          15.0f);
	spotLightCount++;

	// Luz vehiculo
	spotLights[3] = SpotLight(0.0f, 0.5f, 1.0f,
	                          1.0f, 2.0f,
	                          5.0f, 15.0f, 0.0f,
	                          -5.0f, 0.0f, 0.0f,
	                          1.0f, 0.01f, 0.001f,
	                          25.0f);
	spotLightCount++;


	// ========================================= CREACION DE LOS ARREGLOS DE LUCES =============================================
	LightCollectionBuilder<SpotLight> spLightBuilder1(spotLightCount);
	LightCollection<SpotLight> spotLightCollection1 = spLightBuilder1.addFromArray(spotLights, spotLightCount).build();
	LightCollectionBuilder<PointLight> ptLightBuilder1(pointLightCount);
	LightCollection<PointLight> pointLightCollection1 = ptLightBuilder1.addFromArray(pointLights, pointLightCount).build();
	/*
	 *Otra forma de agregar las luces
	 *	LightCollection<SpotLight> spotLightCollection1 = spLightBuilder1.addLight(spotLights[0])
	 *	                                                      .addLight(spotLights[1])
	 *	                                                      .addLight(spotLights[2])
	 *	                                                      .addLight(spotLights[3])
	 *	                                                      .addLight(spotLights[4])
	 *	                                                      .build();
	 *
	 * Idealmente sería para hacer algo como:
	 * ptLightBuilder1.addLight(pointLights[0])
	 *					.addLight(PointLight(0.0f, 1.0f, 1.0f,
	 *										 0.0f, 1.0f,
	 *										 -6.0f, 1.5f, -15.5f,
	 *										 0.3f, 0.2f, 0.1f))
	 *					.build();
	 * Para no tener que crear un arreglo adicional.
	 */

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
	       uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		// informaci?n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c?mara de tipo flash
		// sirve para que en tiempo de ejecuci?n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLightCollection1[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Llamadas para encender o apagar las luces.
		spotLightCollection1.toggleLight(0, mainWindow.getLinterna());
		pointLightCollection1.toggleLight(1, mainWindow.getLampara());

		// informaci?n al shader de fuentes de iluminaci?n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLightCollection1.getLightArray(), pointLightCollection1.getCurrentCount());
		shaderList[0].SetSpotLights(spotLightCollection1.getLightArray(), spotLightCollection1.getCurrentCount());

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//------------ INICIA DIBUJO DEL VEHICULO -------------------
		model = handler.setMatrix(glm::mat4(1.0))
		            .translate(mainWindow.getPosVehiculo(), -1.0f, 0.0f)
		            .rotateY(180.0f)
		            .saveActualState(modelaux) // modelaux = model;
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		car.RenderModel();

		// Luz vehiculo
		model = modelaux;
		glm::vec3 carLightPos = model[3];
		carLightPos.x += 1.5f;
		carLightPos.z += 1.0f;
		spotLightCollection1[3].SetPos(carLightPos);

		// Cofre
		model = handler.setMatrix(modelaux) // model = modelaux;
		            .translate(1.37413, 1.57, 0)
		            .rotateZ(mainWindow.getAnguloCofre())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logofiTexture.UseTexture();
		hood.RenderModel();

		wheelTexture.UseTexture();
		// Rueda derecha trasera
		model = handler.setMatrix(modelaux)
		            .translate(-2.92984, 0.52, 1.28414)
		            .rotateZ(mainWindow.getRotaRuedas())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda izquierda trasera
		model = handler.setMatrix(modelaux)
		            .translate(-2.92984, 0.52, -1.28414)
		            .rotateZ(mainWindow.getRotaRuedas())
		            .rotateX(-180)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda derecha delantera
		model = handler.setMatrix(modelaux)
		            .translate(2.6089, 0.52, 1.28414)
		            .rotateZ(mainWindow.getRotaRuedas())
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Rueda izquierda delantera
		model = handler.setMatrix(modelaux)
		            .translate(2.6089, 0.52, -1.28414)
		            .rotateZ(mainWindow.getRotaRuedas())
		            .rotateX(-180)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wheel.RenderModel();

		// Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getMueveHelicopteroX(), 0.0f + mainWindow.getMueveHelicopteroY(), 6.0));
		// luz
		spotLightCollection1[2].SetPos(model[3]);
		//		spotLights[2].SetPos(model[3]);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// Faro
		model = glm::mat4(1.0);
		model = glm::translate(model, {-15.0f, -1.0f, -4.0f});
		model = glm::rotate(model, glm::radians(90.0f), {0.0f, 1.0f, 0.0f});
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Faro_M.RenderModel();
		model = modelaux;
		model = glm::translate(model, {-6.2, 16.9, 0.0f});
		pointLightCollection1[1].SetPos(model[3]);
		
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(5.0f, 5.0, 5.0)
		            .scale(2.0f, 2.0f, 2.0f)
		            .getMatrix();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // TODO : Meter esta l�nea en ModelMatrix.cpp
		dado8fTexture.UseTexture();
		meshUnorderedMap.at(DADO_8F)->RenderMesh();

		// Agave ?qu? sucede si lo renderizan antes del coche y el helic?ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

#pragma clang diagnostic pop
