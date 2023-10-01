//
// Created by edgar on 9/20/2023.
//

#include "ModelMatrix.h"

ModelMatrix::ModelMatrix(const glm::mat4 &model) : model(model) {}

glm::mat4 ModelMatrix::getMatrix() { return model; }

ModelMatrix &ModelMatrix::setMatrix(const glm::mat4 &origin)
{
	this->model = origin;
	return *this;
}

ModelMatrix &ModelMatrix::addTranslation(float x, float y, float z)
{
	model = glm::translate(model, {x, y, z});
	return *this;
}

ModelMatrix &ModelMatrix::addScale(float x, float y, float z)
{
	model = glm::scale(model, {x, y, z});
	return *this;
}

ModelMatrix &ModelMatrix::saveActualState(glm::mat4 &output)
{
	output = this->model;
	return *this;
}

ModelMatrix &ModelMatrix::addRotationX(float degrees)
{
	model = glm::rotate(model, glm::radians(degrees), {1.0f, 0.0f, 0.0f});
	return *this;
}

ModelMatrix &ModelMatrix::addRotationY(float degrees)
{
	model = glm::rotate(model, glm::radians(degrees), {0.0f, 1.0f, 0.0f});
	return *this;
}

ModelMatrix &ModelMatrix::addRotationZ(float degrees)
{
	model = glm::rotate(model, glm::radians(degrees), {0.0f, 0.0f, 1.0f});
	return *this;
}