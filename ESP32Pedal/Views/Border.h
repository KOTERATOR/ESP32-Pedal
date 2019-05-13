#pragma once

#include "../GFX/View.h"

class Border : public View
{
public:
    Border() : View(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::MATCH_PARENT, ContainerMode::ABSOLUTE, Position(0, 0), Size(0, 0)) {}
    void onDraw()
    {

    }
};