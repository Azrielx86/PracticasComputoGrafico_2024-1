#pragma once

#include "CommonValues.h"
#include "Mesh.h"
#include "Shader_light.h"
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
class Skybox
{
  public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~Skybox();

  private:
	Mesh   *skyMesh;
	Shader *skyShader;
	GLuint  textureId;
	GLuint  uniformProjection, uniformView;
};
