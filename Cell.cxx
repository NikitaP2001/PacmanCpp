#include "Cell.h"

bool Wall::operator<(const Wall& other) const
{
    bool less = false;

    if (xUL != other.xUL || yUL != other.yUL)
        less = xUL < other.xUL || (xUL == other.xUL && yUL < other.yUL);
    else
        less = xRD < other.xRD || (xRD == other.xRD && yRD < other.yRD);
    return less;
}