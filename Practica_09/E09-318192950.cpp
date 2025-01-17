#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Weverything"
#pragma ide diagnostic ignored "readability-non-const-parameter"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "UnusedValue"
#pragma ide diagnostic ignored "modernize-use-emplace"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
/*
Semestre 2024-1
Animaci?n:
Sesi?n 1:
Simple o b?sica:Por banderas y condicionales (m?s de 1 transforomaci?n
geom?trica se ve modificada Sesi?n 2 Compleja: Por medio de funciones y
algoritmos. Adicional.- ,Textura Animada
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelMatrix.h"
#include "Shader_light.h"
#include "Skybox.h"
#include "Texture.h"
#include "Window.h"

// para iluminaci?n
#include "Animation.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PointLight.h"
#include "SpotLight.h"

const float toRadians = 3.14159265f / 180.0f;

// variables para animaci?n
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
// bool avanza;
Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Pinball;
Model CristalPinball;
Model Coin;
Model Canica_1;
Model Canica_2;

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
void calcAverageNormals(unsigned int *indices, unsigned int indiceCount,
                        GLfloat *vertices, unsigned int verticeCount,
                        unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0],
                     vertices[in1 + 1] - vertices[in0 + 1],
                     vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0],
                     vertices[in2 + 1] - vertices[in0 + 1],
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
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1],
                      vertices[nOffset + 2]);
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
    auto *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    ModelMatrix handler(glm::mat4(1.0f));

    camera = Camera(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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

    Kitt_M = Model();
    Kitt_M.LoadModel("Models/kitt_optimizado.obj");
    Llanta_M = Model();
    Llanta_M.LoadModel("Models/llanta_optimizada.obj");
    Blackhawk_M = Model();
    Blackhawk_M.LoadModel("Models/uh60.obj");

    car = Model();
    car.LoadModel("Models/BMW_base.obj");

    hood = Model();
    hood.LoadModel("Models/BMW_hood.obj");

    wheel = Model();
    wheel.LoadModel("Models/BMW_wheel.obj");

    Coin = Model();
    Coin.LoadModel("Models/Coin.obj");
    Pinball = Model();
    Pinball.LoadModel("Models/MaquinaPinball.obj");
    CristalPinball = Model();
    CristalPinball.LoadModel("Models/MaquinaCristal.obj");
    Canica_1 = Model();
    Canica_1.LoadModel("Models/canica.obj");
    Canica_2 = Model();
    Canica_2.LoadModel("Models/canica.obj");

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

    // luz direccional, s?lo 1 y siempre debe de existir
    mainLight =
        DirectionalLight(1.0f, 1.0f, 1.0f, 0.3f, 0.3f, 0.0f, 0.0f, -1.0f);
    // contador de luces puntuales
    unsigned int pointLightCount = 0;
    // Declaraci?n de primer luz puntual
    pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -6.0f, 1.5f, 1.5f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    unsigned int spotLightCount = 0;
    // linterna
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f, 1.0f, 0.01f, 0.0001f, 20.0f);
    spotLightCount++;

    // luz fija
    spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f, 1.0f, 2.0f, 5.0f, 10.0f, 0.0f,
                              0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 15.0f);
    spotLightCount++;

    // se crean mas luces puntuales y spotlight

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
           uniformEyePosition = 0, uniformSpecularIntensity = 0,
           uniformShininess = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f, 1000.0f);
    movCoche = 0.0f;
    movOffset = 0.8f;
    rotllanta = 0.0f;
    rotllantaOffset = 5.0f;
    //    avanza = true;

    float posMonedaX = -4.5;
    float posMonedaY = -1.2;
    float movMonedaOffset = 0.3f;
    float posCanica_1 = 0;
    float posCanica_2 = -0.2;
    float movCanicaOffset = 0.05f;
    float rotaCanica = 0;
    float rotaCanicaOffset = 5.0f;

    // Animacion de la moneda
    Animation coinAnimation;
    coinAnimation
        .addCondition(
            [](float) -> bool
            { return mainWindow.getStartCoinAnimation(); })
        .addCondition(
            [&posMonedaY, &movMonedaOffset](float dt) -> bool
            {
                if (posMonedaY < 3.51984)
                {
                    posMonedaY += movMonedaOffset * dt;
                    return false;
                }
                movMonedaOffset = 0.05f;
                return true;
            })
        .addCondition(
            [&posMonedaX, &movMonedaOffset](float dt) -> bool
            {
                if (posMonedaX < 0)
                {
                    posMonedaX += movMonedaOffset * dt;
                    return false;
                }
                return true;
            })
        .addCondition(
            [&posCanica_1, &movCanicaOffset,
             &rotaCanica, &rotaCanicaOffset, &posCanica_2](float dt) -> bool
            {
                if (posCanica_1 < 1.45229)
                {
                    posCanica_1 += movCanicaOffset * dt;
                    posCanica_2 += movCanicaOffset * dt;
                    rotaCanica += rotaCanicaOffset * dt;
                    return false;
                }
                return true;
            })
        .addCondition(
            [&posMonedaX, &posCanica_1, &posMonedaY, &movMonedaOffset, &posCanica_2](float) -> bool
            {
                if (mainWindow.getResetAnimation())
                {
                    posMonedaX = -4.5;
                    posMonedaY = -1.2;
                    movMonedaOffset = 0.3f;
                    return true;
                }
                return false;
            })
        .addCondition(
            [&posCanica_1, &posCanica_2,
             &movCanicaOffset, &rotaCanicaOffset, &rotaCanica](float dt) -> bool
            {
                if (posCanica_1 > 0)
                {
                    posCanica_1 -= movCanicaOffset * dt;
                    posCanica_2 -= movCanicaOffset * dt;
                    rotaCanica += rotaCanicaOffset * dt;
                    return false;
                }
                return true;
            });
    coinAnimation.prepare();

    Animation carAnimation;
    carAnimation
        .addCondition(
            [](float dt) -> bool
            {
                movCoche -= movOffset * dt;
                rotllanta += rotllantaOffset * dt;
                if (movCoche < -300.0f)
                    return true;
                return false;
            })
        .addCondition(
            [](float dt) -> bool
            {
                movCoche += movOffset * dt;
                rotllanta -= rotllantaOffset * dt;
                if (movCoche > 300.0f)
                    return true;
                return false;
            });
    carAnimation.prepare();

    ////Loop mientras no se cierra la ventana
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        coinAnimation.update(deltaTime);
        carAnimation.update(deltaTime);

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

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                           glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE,
                           glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x,
                    camera.getCameraPosition().y, camera.getCameraPosition().z);

        // luz ligada a la c?mara de tipo flash
        // sirve para que en tiempo de ejecuci?n (dentro del while) se cambien
        // propiedades de la luz
        glm::vec3 lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

        // informaci?n al shader de fuentes de iluminaci?n
        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

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

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model =
            glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Blackhawk_M.RenderModel();

        model = handler
                    .setMatrix(glm::mat4(1.0f))
                    .translate(posMonedaX, posMonedaY, 0.238931)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Coin.RenderModel();

        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(-2.31758, 3.87085, posCanica_1)
                    .rotateX(rotaCanica) // Rotaci�n al moverse
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Canica_1.RenderModel();

        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(-2.31758, 3.87085, posCanica_2)
                    .rotateX(rotaCanica) // Rotaci�n al moverse
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Canica_2.RenderModel();

        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(0.0, -1.0, 0.0)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Pinball.RenderModel();

        // ======================================================== Vehiculo =======================================================================
        model = handler.setMatrix(glm::mat4(1.0))
                    .translate(movCoche, -1.0, -15.0f)
                    .rotateY(180.0f)
                    .saveActualState(modelaux) // modelaux = model;
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        car.RenderModel();
        glDisable(GL_BLEND);

        // Cofre
        model = handler
                    .setMatrix(modelaux) // model = modelaux;
                    .translate(3.23728, 2.75007, 0)
                    .rotateZ(mainWindow.getAnguloCofre())
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        //		logofiTexture.UseTexture();
        hood.RenderModel();

        // Rueda derecha trasera
        model = handler.setMatrix(modelaux)
                    .translate(-3.65932, 1.0601, 2.52948)
                    .rotateZ(-rotllanta)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Rueda izquierda trasera
        model = handler.setMatrix(modelaux)
                    .translate(-3.65932, 1.0601, -2.52948)
                    .rotateZ(-rotllanta)
                    .rotateX(-180)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Rueda derecha delantera
        model = handler.setMatrix(modelaux)
                    .translate(5.19296, 1.05874, 2.52948)
                    .rotateZ(-rotllanta)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();

        // Rueda izquierda delantera
        model = handler.setMatrix(modelaux)
                    .translate(5.19296, 1.05874, -2.52948)
                    .rotateZ(-rotllanta)
                    .rotateX(-180)
                    .scale(0.4, 0.4, 0.4)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        wheel.RenderModel();
        
        model = handler.setMatrix(glm::mat4(1.0f))
                    .translate(0.0, -1.0, 0.0)
                    .getMatrix();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        CristalPinball.RenderModel();
        glDisable(GL_BLEND);

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
