#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Weverything"
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#pragma ide diagnostic ignored "readability-non-const-parameter"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "modernize-use-emplace"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- Textura Animada
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION
#define NUM_DIGITOS 2
#define swordMaxVec 0.08

#define PI_CONST 3.141592654
#define GRAVITY 9.8

#include <cstdio>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Animation.h"
#include "Camera.h"
#include "Displacer.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelMatrix.h"
#include "Shader_light.h"
#include "Skybox.h"
#include "Texture.h"
#include "Window.h"

// para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PointLight.h"
#include "SpotLight.h"
const float toRadians = 3.14159265f / 180.0f;

// variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float toffsetnumerocambiav = 0.0;
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Sword;
Model Canica;

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

// Variables canica
const float masaCanica = 1.0f;

// Vertex Shader
static const char *vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char *fShader = "shaders/shader_light.frag";

/**
 * VTX1 = 0
 * VTX2 = 1
 * FLOOR = 2
 * VEGETACION = 3
 * FLECHA = 4
 * SCORE = 5
 * NUMEROS = 6
 */
enum OBJETOS
{
    VTX1,
    VTX2,
    FLOOR,
    VEGETACION,
    FLECHA,
    SCORE,
    NUMEROS,
    PLANO
};

// cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount,
                        unsigned int vLength, unsigned int normalOffset)
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

// Plano inclinado
    unsigned int planoIdx[] = {
        0, 2, 1,
        1, 2, 3
    };
    
    GLfloat planoVtx[] = {
        -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
        -10.0f, 5.13f, 14.096f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
        10.0f, 5.13f, 14.096f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
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

    Mesh *obj5 = new Mesh();
    obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
    meshList.push_back(obj5);

    Mesh *obj6 = new Mesh();
    obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
    meshList.push_back(obj6);

    Mesh *obj7 = new Mesh();
    obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
    meshList.push_back(obj7);

    auto obj8 = new Mesh();
    obj8->CreateMesh(planoVtx, planoIdx, 32, 6);
    meshList.push_back(obj8);
}

void CreateShaders()
{
    auto shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

    ModelMatrix handler(glm::mat4(1.0f));

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
    FlechaTexture = Texture("Textures/flechas.tga");
    FlechaTexture.LoadTextureA();
    NumerosTexture = Texture("Textures/numerosbase.tga");
    NumerosTexture.LoadTextureA();
    Numero1Texture = Texture("Textures/numero1.tga");
    Numero1Texture.LoadTextureA();
    Numero2Texture = Texture("Textures/numero2.tga");
    Numero2Texture.LoadTextureA();
#if ELEMENTOS_EJERCICIO
    Kitt_M = Model();
    Kitt_M.LoadModel("Models/kitt_optimizado.obj");
    Llanta_M = Model();
    Llanta_M.LoadModel("Models/llanta_optimizada.obj");
    Blackhawk_M = Model();
    Blackhawk_M.LoadModel("Models/uh60.obj");
#endif
    Sword = Model();
    Sword.LoadModel("Models/sword.obj");
    Canica = Model();
    Canica.LoadModel("Models/canica.obj");

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

    // luz direccional, sólo 1 y siempre debe de existir
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
                                 0.3f, 0.3f,
                                 0.0f, 0.0f, -1.0f);
    // contador de luces puntuales
    unsigned int pointLightCount = 0;
    // Declaración de primer luz puntual
    pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
                                0.0f, 1.0f,
                                0.0f, 2.5f, 1.5f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    unsigned int spotLightCount = 0;
    // linterna
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              15.0f);
    spotLightCount++;

    // luz fija
    spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
                              1.0f, 2.0f,
                              5.0f, 10.0f, 0.0f,
                              0.0f, -5.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              15.0f);
    spotLightCount++;

    // luz de helicóptero

    // luz de faro

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
           uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

    movCoche = 0.0f;
    movOffset = 0.01f;
    rotllanta = 0.0f;
    rotllantaOffset = 10.0f;
#if ELEMENTOS_EJERCICIO
    bool intercambia = true;
    int sigNumCambio = 2;
    int siguienteCambio = 5;
    int contadorNumeros = 0;
    int contador = 0;

    int numberTimeout = 2.0f;
    int sinceLastAction = 0;
#endif
    // Seccion para el lanzamiento de la espada
    glm::vec3 swordPos(60.0f, 13.0f, 0.0f);
    float swordDesp = 0.0;
    float swordT = 0.0f;
    glm::vec3 rotSword = {-115.0f, 0.0f, 90.0f};
    float accSword = 0.0;

    Animation swordAnimation;
    swordAnimation
        .addCondition(
            [&rotSword](float dt) -> bool
            { return mainWindow.getsKeys()[GLFW_KEY_P]; })
        .addCondition(
            [&swordPos, &rotSword](float dt) -> bool
            {
                if (rotSword.x < 0)
                {
                    rotSword.x += 20.0 * dt;
                    rotSword.z -= 15.0 * dt;
                    return false;
                }
                return true;
            })
        .addCondition(
            [&swordPos, &accSword,
             &swordDesp, &swordT, &rotSword](float dt) -> bool
            {
                float dsp = swordMaxVec * dt;
                float acc = dsp * dsp * dt;

                if (swordDesp < swordMaxVec)
                    swordDesp += acc * dt;
                swordT += dsp;
                swordPos.x = 30 + 30 * cos(swordT);
                swordPos.z = 24 * sin(swordT);
                swordPos.x = swordPos.x + 1.5 * cos(swordT * 10);
                swordPos.z = swordPos.z + 1.5 * sin(swordT * 10);
                rotSword.y += 10 * dt;

                if (swordT >= 2 * PI_CONST)
                    return true;
                return false;
            })
        .addCondition(
            [&swordT, &swordPos, &rotSword](float) -> bool
            {
                swordT = 0;
                swordPos = glm::vec3(60.0f, 13.0f, 0.0f);
                rotSword = {0.0f, 0.0f, 0.0f};
                return true;
            })
        .addCondition(
            [&swordPos, &rotSword](float dt) -> bool
            {
                if (rotSword.x > -115)
                {
                    rotSword.x -= 20.0 * dt;
                    rotSword.z += 15.0 * dt;
                    return false;
                }
                return true;
            })
        .addCondition(
            [&rotSword](float dt) -> bool
            {
                rotSword = {-115.0f, 0.0f, 90.0f};
                return true;
            })
        .prepare();

    // Sección para el lanzamiento de la canica
    glm::vec3 posCanica(-14, 1, 0);

#if EXTRA_CANICA
    Animation canicaAnimation;
    canicaAnimation
        .addCondition(
            [](float) -> bool
            {
                return mainWindow.getsKeys()[GLFW_KEY_U];
            })
        .addCondition(
            [&posCanica](float dt) -> bool
            {
                // Hubiera puesto atención en las clases de fisica... :P
                float inercia = (2.0 / 5.0) * masaCanica * pow(0.5, 2);
                float a = (GRAVITY * sin(20)) / (1.0 + (inercia / (masaCanica * pow(0.5, 2))));
                
                posCanica.z -= a;
                posCanica.y = posCanica.z * tan(20);
                
                if (posCanica.y <= 0) return true;
                else return false;
            })
        .prepare();
#endif
    
    // Sección para el contador
    std::vector<Displacer *> digitCounter;
    float nextDigitsChange = 1;

    // Con este for se puede ajustar el número de dígitos
    for (int i = 0; i < NUM_DIGITOS; ++i)
    {
        auto displacer = new Displacer(3, 2);
        displacer->setInitialCoordinates(1, 2);
        if (i > 0)
            displacer->connectCounter(digitCounter.at(i - 1));
        digitCounter.push_back(displacer);
    }

    auto rootCounter = digitCounter.back();

    ////Loop mientras no se cierra la ventana
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;
        printf("Tiempo de ejecucion: %f\r", glfwGetTime());

        swordAnimation.update(deltaTime);
#if EXTRA_CANICA
        canicaAnimation.update(deltaTime);
#endif

        if (glfwGetTime() > nextDigitsChange)
        {
            rootCounter->increment();
            nextDigitsChange = glfwGetTime() + 0.2;
        }

        angulovaria += 0.5f * deltaTime;

        if (movCoche < 30.0f)
        {
            movCoche -= movOffset * deltaTime;
        }
        rotllanta += rotllantaOffset * deltaTime;

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
        uniformTextureOffset = shaderList[0].getOffsetLocation();

        // información en el shader de intensidad especular y brillo
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        // luz ligada a la cámara de tipo flash
        glm::vec3 lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

        // información al shader de fuentes de iluminación
        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glm::mat4 model(1.0);
        glm::mat4 modelaux(1.0);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

        meshList[2]->RenderMesh();

        // Instancia de la espada
        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(swordPos)
                    .rotateX(rotSword.x)
                    .rotateY(rotSword.y)
                    .rotateZ(rotSword.z)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Sword.RenderModel();
#if EXTRA_CANICA
        // Plano y canica
        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(-30, -1, 7)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[OBJETOS::PLANO]->RenderMesh();

        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(-14 + posCanica.x, posCanica.y, posCanica.z)
                    .scale(6.67)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Canica.RenderModel();
#endif
#ifdef ELEMENTOS_EJERCICIO
        // Instancia del coche
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(movCoche - 50.0f, 0.5f, -2.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Kitt_M.RenderModel();

        // Llanta delantera izquierda
        model = modelaux;
        model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        color = glm::vec3(0.5f, 0.5f, 0.5f); // llanta con color gris
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Llanta_M.RenderModel();

        // Llanta trasera izquierda
        model = modelaux;
        model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Llanta_M.RenderModel();

        // Llanta delantera derecha
        model = modelaux;
        model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Llanta_M.RenderModel();

        // Llanta trasera derecha
        model = modelaux;
        model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Llanta_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f + movCoche, 5.0f + sin(glm::radians(angulovaria)), 6.0));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        // color = glm::vec3(0.0f, 1.0f, 0.0f);
        // glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Blackhawk_M.RenderModel();

        // Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // blending: transparencia o traslucidez
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        AgaveTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[3]->RenderMesh();

        // textura con movimiento
        // Importantes porque la variable uniform no podemos modificarla directamente
        toffsetflechau += 0.001;
        toffsetflechav += 0.0;
        // para que no se desborde la variable
        if (toffsetflechau > 1.0)
            toffsetflechau = 0.0;
        // if (toffsetv > 1.0)
        //	toffsetv = 0;
        // printf("\ntfosset %f \n", toffsetu);
        // pasar a la variable uniform el valor actualizado
        toffset = glm::vec2(toffsetflechau, toffsetflechav);

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        FlechaTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[4]->RenderMesh();

        // plano con todos los números
        toffsetnumerou = 0.0;
        toffsetnumerov = 0.0;
        toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        NumerosTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[5]->RenderMesh();

        // número 1
        // toffsetnumerou = 0.0;
        // toffsetnumerov = 0.0;
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        // glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        NumerosTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[OBJETOS::NUMEROS]->RenderMesh();

        for (int i = 1; i < 4; i++)
        {
            // números 2-4
            toffsetnumerou += 0.25;
            toffsetnumerov = 0.0;
            toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
            model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
            glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            color = glm::vec3(1.0f, 1.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            NumerosTexture.UseTexture();
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            meshList[6]->RenderMesh();
        }

        for (int j = 1; j < 5; j++)
        {
            // números 5-8
            toffsetnumerou += 0.25;
            toffsetnumerov = -0.33;
            toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
            model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
            glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            color = glm::vec3(1.0f, 1.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            NumerosTexture.UseTexture();
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            meshList[6]->RenderMesh();
        }

        // número cambiante
        /*
        ¿Cómo hacer para que sea a una velocidad visible?
        */
        if (siguienteCambio <= (int)glfwGetTime())
        {
            toffsetnumerocambiau += 0.25;
            if (toffsetnumerocambiau >= 1.0)
            {
                toffsetnumerocambiau = 0.0;
                contadorNumeros--;
                if (contadorNumeros < 0)
                    contadorNumeros = 2;
            }

            /*
                Valores no válidos
                Para no dibujar las partes de la textura que no corresponden a un numero.
                (u, v) = (0.500000, 0.330000)
                (u, v) = (0.750000, 0.330000)
            */
            if (contadorNumeros == 1 && toffsetnumerocambiau >= 0.5)
            {
                toffsetnumerocambiau = 0.0;
                contadorNumeros = 0;
            }
            toffsetnumerocambiav = 0.33 * contadorNumeros;
            siguienteCambio = (int)glfwGetTime() + 1;
        }

        //        printf("(%f, %f)\n", toffsetnumerocambiau, toffsetnumerocambiav);

        toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerocambiav);
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        NumerosTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[6]->RenderMesh();

        // cambiar automáticamente entre textura número 1 y número 2
        toffsetnumerou = 0.0;
        toffsetnumerov = 0.0;
        toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        Numero1Texture.UseTexture();
        // if
        if (sigNumCambio <= (int)glfwGetTime())
        {
            sigNumCambio = (int)glfwGetTime() + 2;
            intercambia = !intercambia;
        }

        if (intercambia)
            Numero1Texture.UseTexture();
        else
            Numero2Texture.UseTexture();

        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[5]->RenderMesh();
#else
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

        for (int i = 0; i < digitCounter.size(); i++)
        {
            auto digito = digitCounter.at(i);
            toffset = {digito->getU() * 0.25, digito->getV() * (-0.33)};
            model = handler.setMatrix(glm::mat4(1.0f))
                        .translate(10 + (i * 3.0), 2.0, -6.0)
                        .rotateX(90)
                        .scale(3.0)
                        .getMatrix();
            glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            color = glm::vec3(1.0f, 1.0f, 1.0f);
            NumerosTexture.UseTexture();
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            meshList[6]->RenderMesh();
        }

        glDisable(GL_BLEND);

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}

#pragma clang diagnostic pop