#pragma once

#include "Dimensions.h"

class Container
{
protected:
    Container * parent;
    Position position;
    Size size;

public:
    Position getPosition();
    Position getAbsolutePosition();
    Size getSize();
};

Position Container::getPosition()
{
    return position;
}

Position Container::getAbsolutePosition()
{
    Position pos;
    if(parent != nullptr)
    {
        pos = parent->getAbsolutePosition();
    }
}