#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge      = 0.0f;
	procEdge  = cosf(glm::radians(edge));
}

SpotLight::SpotLight(glm::vec3 rgb,
                     GLfloat   aIntensity,
                     GLfloat   dIntensity,
                     glm::vec3 pos,
                     glm::vec3 dir,
                     GLfloat   con,
                     GLfloat   lin,
                     GLfloat   exp,
                     GLfloat   edg)
    : PointLight(rgb, aIntensity, dIntensity, pos, con, lin, exp)
{
	direction = glm::normalize(dir);
	edge      = edg;
	edge  = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation,
                         GLuint ambientColourLocation,
                         GLuint diffuseIntensityLocation,
                         GLuint positionLocation,
                         GLuint directionLocation,
                         GLuint constantLocation,
                         GLuint linearLocation,
                         GLuint exponentLocation,
                         GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, edge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position  = pos;
	direction = dir;
}

void SpotLight::SetPos(glm::vec3 pos) { position = pos; }

void SpotLight::SetExp(GLfloat exp) { this->exponent = exp; }

void SpotLight::SetDirection(glm::vec3 dir) { this->direction = dir; }

void SpotLight::SetConstant(GLfloat con) { this->constant = con; }

void SpotLight::SetLinear(GLfloat lin) { this->linear = lin; }

void SpotLight::SetEdg(GLfloat edg) { this->edge = cosf(glm::radians(edg));; }

void SpotLight::SetColor(glm::vec3 color) { this->color = color; }

void SpotLight::SetIntensity(GLfloat ambient, GLfloat diffuse)
{
	this->ambientIntensity = ambient;
	this->diffuseIntensity = diffuse;
}

SpotLight::~SpotLight() {}
