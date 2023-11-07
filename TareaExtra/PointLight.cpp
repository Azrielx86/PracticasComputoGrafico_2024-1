#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(glm::vec3 rgb,
                       GLfloat aIntensity,
                       GLfloat dIntensity,
                       glm::vec3 pos,
                       GLfloat con,
                       GLfloat lin,
                       GLfloat exp) : Light(rgb, aIntensity, dIntensity)
{
	position = pos;
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

void PointLight::SetConstant(GLfloat con) { this->constant = con; }

void PointLight::SetLinear(GLfloat lin) { this->linear = lin; }

void PointLight::SetExp(GLfloat exp) { this->exponent = exp; }

void PointLight::SetPos(glm::vec3 pos) { position = pos; }

PointLight::~PointLight()
{
}
const glm::vec3 &PointLight::getPosition() const
{
	return position;
}
GLfloat PointLight::getConstant() const
{
	return constant;
}
GLfloat PointLight::getLinear() const
{
	return linear;
}
GLfloat PointLight::getExponent() const
{
	return exponent;
}
