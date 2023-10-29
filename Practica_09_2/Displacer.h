//
// Created by edgar on 10/28/2023.
//

#ifndef PRACTICA09_DISPLACER_H
#define PRACTICA09_DISPLACER_H

#include "Counter.h"

/**
 * Clase para calcular las coordenadas u y v de una textura con offset.
 */
class Displacer : public Counter
{
  public:
    Displacer(short u, short v);
    [[nodiscard]] short getU() const;
    [[nodiscard]] short getV() const;
    void setInitialCoordinates(short u, short v);
    void increment() override;
    
  private:
    short uThreshold, vThreshold;
    short u, v;
};

#endif // PRACTICA09_DISPLACER_H
