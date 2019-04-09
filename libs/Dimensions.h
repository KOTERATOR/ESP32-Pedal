#pragma once

#include "Vector2.h"

class Position : public Vector2
{
public:
    Position operator+(Position rhs);
    Position operator-(Position rhs);
};

Position Position::operator+(Position rhs)
{
    Position pos;
    pos.x = this->x + rhs.x;
    pos.y = this->y + rhs.y;
    return pos;
}

Position Position::operator-(Position rhs)
{
    Position pos;
    pos.x = this->x - rhs.x;
    pos.y = this->y - rhs.y;
    return pos;
}

Position Position::operator-(Position rhs)
{

}

class Size
{
public:
    int width = 0, height = 0;
    
    Size operator+(Size rhs);
    Size operator-(Size rhs);
};

Size Size::operator+(Size rhs)
{
    Size size;
    size.width = this->width + rhs.width;
    size.height = this->height + rhs.height;
    return size;
}

Size Size::operator-(Size rhs)
{
    Size size;
    size.width = this->width - rhs.width;
    size.height = this->height - rhs.height;
    return size;
}