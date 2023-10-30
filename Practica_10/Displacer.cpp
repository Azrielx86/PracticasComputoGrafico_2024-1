//
// Created by edgar on 10/28/2023.
//

#include "Displacer.h"
Displacer::Displacer(short u, short v) : uThreshold(u), vThreshold(v), u(0), v(0) {}
short Displacer::getU() const { return u; }
short Displacer::getV() const { return v; }
void Displacer::increment()
{
    Counter::increment();
    u++;
    
    if (v == 2 && u >= 2)
    {
        u = v = 0;
        return;
    }
    if (u > uThreshold)
    {
        v++;
        u = 0;
        if (v > vThreshold)
            v = 0;
    }

}
void Displacer::setInitialCoordinates(short u, short v)
{
    this->u = u;
    this->v = v;
}
