//
// Created by edgar on 9/20/2023.
//

#ifndef MAIN_MODELMATRIX_H
#define MAIN_MODELMATRIX_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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
    ModelMatrix &setMatrix(const glm::mat4 &origin);

    /**
     * Agrega una traslación a la matriz.
     */
    ModelMatrix &translate(float x, float y, float z);

    /**
     * Agrega una traslación a la matriz.
     */
    ModelMatrix &translate(const glm::vec3 &pos);

    /**
     * Agrega una escala a la matriz.
     */
    ModelMatrix &scale(float x, float y, float z);

    /**
     * Agrega una escala a la matriz igual en todos los ejes.
     * @param size
     * @return
     */
    ModelMatrix &scale(float size);

    /**
     * Guarda el estado actual de la matriz.
     * @param output Referencia a la matriz donde se guardara la matriz.
     */
    ModelMatrix &saveActualState(glm::mat4 &output);

    /**
     * Agrega una rotación en el eje x a la matriz
     */
    ModelMatrix &rotateX(float degrees);

    /**
     * Agrega una rotación en el eje y a la matriz
     */
    ModelMatrix &rotateY(float degrees);

    /**
     * Agrega una rotación en el eje z a la matriz
     */
    ModelMatrix &rotateZ(float degrees);
};

#endif // MAIN_MODELMATRIX_H
