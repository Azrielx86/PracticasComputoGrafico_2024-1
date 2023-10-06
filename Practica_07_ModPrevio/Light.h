#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
  public:
	Light();
	Light(glm::vec3 rgb, GLfloat aIntensity, GLfloat dIntensity);

	~Light();

  protected:
	glm::vec3 color;
	GLfloat   ambientIntensity;
	GLfloat   diffuseIntensity;
};
