//
// Created by edgar on 9/20/2023.
//

#ifndef MAIN_MODELMATRIX_H
#define MAIN_MODELMATRIX_H

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

class ModelMatrix
{
  private:
	glm::mat4 model{};

  public:
	explicit ModelMatrix(const glm::mat4 &model);

  public:
	glm::mat4 getMatrix();
	ModelMatrix &setMatrix(glm::mat4 &origin);
	ModelMatrix &addTranslation(float x, float y, float z);
	ModelMatrix &addScale(float x, float y, float z);
	ModelMatrix &saveActualState(glm::mat4& output);
	ModelMatrix &addRotationX(float degrees);
	ModelMatrix &addRotationY(float degrees);
	ModelMatrix &addRotationZ(float degrees);
};

#endif // MAIN_MODELMATRIX_H
