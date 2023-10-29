//
// Created by edgar on 10/27/2023.
//

#include "Counter.h"
Counter::Counter()
{
    currentCount = 0;
    threshold = 9;
    connCounter = nullptr;
}

void Counter::connectCounter(Counter *counter) { connCounter = counter; }

void Counter::increment()
{
    currentCount++;
    if (currentCount > threshold)
    {
        currentCount = 0;
        if (connCounter != nullptr)
            connCounter->increment();
    }
}
short Counter::getCurrentCount() const
{
    return currentCount;
}
