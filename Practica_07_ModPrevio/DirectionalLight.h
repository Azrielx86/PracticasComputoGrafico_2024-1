#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
  public:
	DirectionalLight();
	DirectionalLight(glm::vec3 rgb,
	                 GLfloat aIntensity,
	                 GLfloat dIntensity,
	                 glm::vec3 dir);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~DirectionalLight();

  private:
	glm::vec3 direction;
};
