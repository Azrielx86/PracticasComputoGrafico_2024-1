#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 rgb, GLfloat aIntensity, GLfloat dIntensity)
{
	color = rgb;
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

void Light::SetColor(glm::vec3 color) { this->color = color; }


Light::~Light()
{
}
