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
/* Semestre 2024-1
Animaci�n por keyframes
La textura del skybox fue conseguida desde la p�gina https ://opengameart.org/content/elyvisions-skyboxes?page=1
y edit� en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
    https ://www.khronos.org/opengl/wiki/Keyframe_Animation
    http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
    */
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Camera.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "KeyFrameAnimation.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelMatrix.h"
#include "PointLight.h"
#include "Shader_light.h"
#include "Skybox.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Window.h"

// Librería para JSON
// https://github.com/nlohmann/json
#include "LightCollection.h"
#include "json.hpp"

using json = nlohmann::json;

const float toRadians = 3.14159265f / 180.0f;

// variables para animaci�n
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Skybox skybox;

float anguloRuedas;
Model car, wheel;
Model cone;

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

KeyFrameAnimation carAnimation;
Entity carEntity;

// Vertex Shader
static const char *vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char *fShader = "shaders/shader_light.frag";

// c�lculo del promedio de las normales para sombreado de Phong
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

    camera = Camera(glm::vec3(0.0f, 60.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

    ModelMatrix handler(glm::mat4(1.0f));

    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTextureA();
    pisoTexture = Texture("Textures/piso.tga");
    pisoTexture.LoadTextureA();

    //    Entity CanicaEntity;
    //    CanicaEntity.declareControl(Entity::RET, GLFW_KEY_DOWN);
    //    CanicaEntity.declareControl(Entity::WALK, GLFW_KEY_UP);
    //    CanicaEntity.declareControl(Entity::LEFT, GLFW_KEY_LEFT);
    //    CanicaEntity.declareControl(Entity::RIGHT, GLFW_KEY_RIGHT);
    //    CanicaEntity.declareControl(Entity::UP, GLFW_KEY_SPACE);
    //    CanicaEntity.declareControl(Entity::DOWN, GLFW_KEY_LEFT_SHIFT);

    std::vector<std::string> skyboxFaces;

#ifdef SKYBOX_SP
    skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
    skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
    skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
    skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
    skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
    skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");
    skybox = Skybox(skyboxFaces);
#else
    // https://sketchfab.com/3d-models/sky-box-8k-night-forest-scene-with-aurora-sky-a626c2f3eda14177b07f9c345a17df60
    skyboxFaces.push_back("Textures/Skybox/nx.png");
    skyboxFaces.push_back("Textures/Skybox/px.png");
    skyboxFaces.push_back("Textures/Skybox/ny.png");
    skyboxFaces.push_back("Textures/Skybox/py.png");
    skyboxFaces.push_back("Textures/Skybox/nz.png");
    skyboxFaces.push_back("Textures/Skybox/pz.png");
    skybox = Skybox(skyboxFaces, true);
#endif

    car = Model();
    car.LoadModel("Models/BMW.obj");

    wheel = Model();
    wheel.LoadModel("Models/BMW_wheel.obj");

    cone = Model();
    cone.LoadModel("Models/cone.obj");

    Material_brillante = Material(4.0f, 256);
    Material_opaco = Material(0.3f, 4);

    // luz direccional, s�lo 1 y siempre debe de existir
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
                                 0.5f, 0.3f,
                                 0.0f, 0.0f, -1.0f);
    // contador de luces puntuales
    unsigned int pointLightCount = 0;
    // Declaraci�n de primer luz puntual
    //    pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
    //                                0.0f, 1.0f,
    //                                0.0f, 2.5f, 1.5f,
    //                                0.3f, 0.2f, 0.1f);
    //    pointLightCount++;

    unsigned int spotLightCount = 0;
    // linterna
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              5.0f);
    spotLightCount++;

    spotLights[1] = SpotLight(0.8f, 0.8f, 1.0f,
                              1.0f, 2.0f,
                              5.0f, 15.0f, 0.0f,
                              -5.0f, 0.0f, 0.0f,
                              1.0f, 0.05f, 0.002f,
                              35.0f);

    spotLightCount++;

    spotLights[2] = SpotLight(0.8f, 0.8f, 1.0f,
                              1.0f, 2.0f,
                              -5.0f, 15.0f, 0.0f,
                              -5.0f, 0.0f, 0.0f,
                              1.0f, 0.05f, 0.002f,
                              35.0f);

    spotLightCount++;

    LightCollectionBuilder<SpotLight> spLightBuilder1(spotLightCount);
    LightCollection<SpotLight> spotLightCollection1 = spLightBuilder1.addFromArray(spotLights, spotLightCount).build();

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
           uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

    movCoche = 0.0f;
    movOffset = 0.01f;
    rotllanta = 0.0f;
    rotllantaOffset = 10.0f;
    glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

    /*
     * Ejemplo para leer desde un archivo
     * KeyFrameAnimation helicAnimation;
     * helicAnimation.loadFromFile("HelicopterAnimation.json");
     */

    carAnimation.loadFromFile("CarAnimation.json");

    /*
     * Controles:
     * M - Modo captura o Modo ejecutar animación.
     * == Modo Captura ==
     * C - Guarda Frame
     * R - Elimina Frame
     * == Modo animacion ==
     * Space - Ejecuta animación.
     * R - Reinicia animación.
     */
    bool modoCaptura = false;

    mainWindow
        .addCallback(
            GLFW_KEY_UP,
            [&modoCaptura]() -> void
            {
                if (!modoCaptura)
                    return;
                carEntity.moveX(10 * deltaTime);
            },
            true)
        .addCallback(
            GLFW_KEY_LEFT,
            [&modoCaptura]() -> void
            {
                if (!modoCaptura)
                    return;
                carEntity.moveZ(-10 * deltaTime);
            },
            true)
        .addCallback(
            GLFW_KEY_DOWN,
            [&modoCaptura]() -> void
            {
                if (!modoCaptura)
                    return;
                carEntity.moveX(-10 * deltaTime);
            },
            true)
        .addCallback(
            GLFW_KEY_RIGHT,
            [&modoCaptura]() -> void
            {
                if (!modoCaptura)
                    return;
                carEntity.moveZ(10 * deltaTime);
            },
            true)
        .addCallback(
            GLFW_KEY_K,
            [&modoCaptura]() -> void
            {
                if (!modoCaptura)
                    return;
                carEntity.rotateY(25 * deltaTime);
            },
            true)
        .addCallback(
            GLFW_KEY_L,
            [&modoCaptura]() -> void
            {
                if (!modoCaptura)
                    return;
                carEntity.rotateY(-25 * deltaTime);
            },
            true)
        .addCallback(GLFW_KEY_M,
                     [&modoCaptura]() -> void
                     {
                         modoCaptura = !modoCaptura;
                         printf("Modo captura: %s\n", modoCaptura ? "true" : "false");
                     })
        // Callbacks del modo captura
        .addCallback(GLFW_KEY_C,
                     [&modoCaptura]() -> void
                     {
                         if (!modoCaptura)
                             return;
                         printf("Frame capturado\n");
                         carAnimation.addKeyframe(carEntity.getPosition(), carEntity.getRotation());
                     })
        .addCallback(GLFW_KEY_R,
                     [&modoCaptura] -> void
                     {
                         if (!modoCaptura)
                             return;
                         printf("Frame Eliminado\n");
                         carAnimation.removeLastFrame();
                     })
        .addCallback(GLFW_KEY_G,
                     [&modoCaptura] -> void
                     {
                         if (!modoCaptura)
                             return;
                         printf("Animacion guardada.\n");
                         carAnimation.saveToFile("CarAnimation.json");
                     })
        // Callbacks del modo reproduccion
        .addCallback(GLFW_KEY_SPACE,
                     [&modoCaptura]() -> void
                     {
                         if (modoCaptura)
                             return;
                         if (!carAnimation.isPlaying())
                             carAnimation.play();
                     });

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

        //        // Control entidades
        //        if (modoCaptura)
        //            carEntity.move(mainWindow.getsKeys(), deltaTime);

        if (carAnimation.isPlaying())
        {
            anguloRuedas += 25;
            carAnimation.play();
        }
        else
            carAnimation.resetAnimation();

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

        // informaci�n en el shader de intensidad especular y brillo
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        // luz ligada a la c�mara de tipo flash
        glm::vec3 lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

        // informaci�n al shader de fuentes de iluminaci�n
        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLightCollection1.getLightArray(), spotLightCollection1.getCurrentCount());

        glm::mat4 model(1.0);
        glm::mat4 modelaux(1.0);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

        model = handler.setMatrix(glm::mat4(1.0))
                    .translate(modoCaptura ? carEntity.getPosition() : carAnimation.getPosition() + carAnimation.getMovement())
                    .rotateY(modoCaptura ? carEntity.getRotation().y : carAnimation.getRotation().y)
                    .saveActualState(modelaux) // modelaux = model;
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        car.RenderModel();
        glDisable(GL_BLEND);

        // Rueda derecha trasera
        model = handler.setMatrix(modelaux)
                    .translate(-3.65932, 1.0601, 2.52948)
                    .rotateZ(anguloRuedas)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Rueda izquierda trasera
        model = handler.setMatrix(modelaux)
                    .translate(-3.65932, 1.0601, -2.52948)
                    .rotateZ(anguloRuedas)
                    .rotateX(-180)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Rueda derecha delantera
        model = handler.setMatrix(modelaux)
                    .translate(5.19296, 1.05874, 2.52948)
                    .rotateZ(anguloRuedas)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Rueda izquierda delantera
        model = handler.setMatrix(modelaux)
                    .translate(5.19296, 1.05874, -2.52948)
                    .rotateZ(anguloRuedas)
                    .rotateX(-180)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Conos
        for (int i = 0; i < 5; ++i)
        {
            model = handler.setMatrix(glm::mat4(1.0f))
                        .translate(10 + (25 * i), 0, 3 * sin(i))
                        .scale(2)
                        .getMatrix();
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            cone.RenderModel();
        }

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}

#pragma clang diagnostic pop
