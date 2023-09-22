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
	/**
	 * Recupera la matriz con las transformaciones realizadas 
	 */
	glm::mat4 getMatrix();
	
	/**
	 * Establece la matriz de inicio
	 * @param origin Referencia a una matriz de origen
	 */
	ModelMatrix &setMatrix(glm::mat4 &origin);
	
	/**
	 * Agrega una traslación a la matriz.
	 */
	ModelMatrix &addTranslation(float x, float y, float z);
	
	/**
	 * Agrega una escala a la matriz.
	 */
	ModelMatrix &addScale(float x, float y, float z);
	
	/**
	 * Guarda el estado actual de la matriz.
	 * @param output Referencia a la matriz donde se guardara la matriz.
	 */
	ModelMatrix &saveActualState(glm::mat4& output);
	
	/**
	 * Agrega una rotación en el eje x a la matriz
	 */
	ModelMatrix &addRotationX(float degrees);
	
	/**
	 * Agrega una rotación en el eje y a la matriz
	 */
	ModelMatrix &addRotationY(float degrees);
	
	/**
	 * Agrega una rotación en el eje z a la matriz
	 */
	ModelMatrix &addRotationZ(float degrees);
};

#endif // MAIN_MODELMATRIX_H
