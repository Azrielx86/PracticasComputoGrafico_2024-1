//
// Created by edgar on 10/27/2023.
//

#ifndef PRACTICA09_COUNTER_H
#define PRACTICA09_COUNTER_H

#include <functional>
class Counter
{
  public:
    Counter();
    void setThreshold(const std::function<void()>& incFunction);
    void connectCounter(Counter* counter);
    
  private:
    short currentCount;
    short threshold;
    Counter* connCounter;
    std::function<void()> increase;
};

#endif // PRACTICA09_COUNTER_H
