#pragma once
#include "PointLight.h"
class SpotLight : public PointLight
{
  public:
	SpotLight();

	SpotLight(glm::vec3 rgb,
	          GLfloat   aIntensity,
	          GLfloat   dIntensity,
	          glm::vec3 pos,
	          glm::vec3 dir,
	          GLfloat   con,
	          GLfloat   lin,
	          GLfloat   exp,
	          GLfloat   edg);

	void UseLight(GLuint ambientIntensityLocation,
	              GLuint ambientColourLocation,
	              GLuint diffuseIntensityLocation,
	              GLuint positionLocation,
	              GLuint directionLocation,
	              GLuint constantLocation,
	              GLuint linearLocation,
	              GLuint exponentLocation,
	              GLuint edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	void SetPos(glm::vec3 pos);
	void SetDirection(glm::vec3 dir);
	void SetIntensity(GLfloat ambient, GLfloat diffuse);
	void SetEdge(GLfloat edg);
//	void SetColor(glm::vec3 color);
	~SpotLight();

  private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};
