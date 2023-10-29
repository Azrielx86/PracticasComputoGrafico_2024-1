//
// Created by edgar on 10/27/2023.
//

#ifndef PRACTICA09_COUNTER_H
#define PRACTICA09_COUNTER_H

#include <functional>

/**
 * Clase contador base, podría ser util para otros contextos más adelante.
 * Quizas...
 */
class Counter
{
  public:
    Counter();
    
    /**
     * Conecta el contador con otro, para enlazarlos.
     * Inspirado en una practica de VHDL :P 
     * @param counter 
     */
    void connectCounter(Counter* counter);
    
    /**
     * Incrementa en 1 el contador
     */
    virtual void increment();
  
    /**
     * @return Valor actual del contador. 
     */
    [[nodiscard]] short getCurrentCount() const;

  protected:
    short currentCount;

  private:
    short threshold;
    Counter* connCounter;
};

#endif // PRACTICA09_COUNTER_H
