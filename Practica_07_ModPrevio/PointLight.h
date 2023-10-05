#pragma once
#include "Light.h"

class PointLight : public Light
{
  public:
	PointLight();
	PointLight(glm::vec3 rgb,
	           GLfloat aIntensity,
	           GLfloat dIntensity,
	           glm::vec3 pos,
	           GLfloat con,
	           GLfloat lin,
	           GLfloat exp);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

	~PointLight();

  protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};
