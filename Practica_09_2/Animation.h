//
// Created by edgar on 10/17/2023.
//

#ifndef PRACTICA08_ANIMATION_H
#define PRACTICA08_ANIMATION_H

#include <functional>
#include <vector>

/**
 * Clase que controla una animación secuencial mediante funciones
 */
class Animation
{
  public:
    Animation();
    /**
     * Agrega una funcion a la animacion completa.
     * @param function El parametro flotante es el deltaTime, la funcion debe retornar un booleano.
     * @return Referencia a la instancia del objeto.
     */
    Animation &addCondition(const std::function<bool(float)> &function);
    
    /**
     * Prepara la animacion estableciendo la primera funcion agregada como la que empezara.
     */
    void prepare();
    
    /**
     * Actualiza el estado de la animacion.
     * @param deltaTime dt, enviado desde el bucle principal.
     */
    void update(float deltaTime);

  private:
    int currentIndex;
    std::function<bool(float)> *current;
    std::vector<std::function<bool(float)>> conditions;
};

#endif // PRACTICA08_ANIMATION_H
