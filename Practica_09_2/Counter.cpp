//
// Created by edgar on 10/27/2023.
//

#include "Counter.h"
Counter::Counter()
{
    currentCount = 0;
    threshold = 9;
    connCounter = nullptr;
    increase = [this]()
    {
        currentCount++;
        if (currentCount == threshold)
        {
            currentCount++;
        }
    };
}


void Counter::connectCounter(Counter *counter){ connCounter = counter; }

void Counter::setThreshold(const std::function<void()> &incFunction){ increase = incFunction; }
