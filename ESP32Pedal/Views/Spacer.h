#pragma once

#include "../GFX/View.h"

class Spacer : public View
{
  public:
    Spacer(Size size, Position pos = Position(0, 0)) : View(ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, ContainerMode::NORMAL, pos, size) {}

    void onDraw(ViewGFX * gfx)
    {
    }
};