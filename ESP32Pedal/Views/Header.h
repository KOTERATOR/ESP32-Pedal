#pragma once

#include "../GFX/View.h"

class Header : public View
{
  private:
    String name;

  public:
    Header(String name) : View(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::FIXED, ContainerMode::NORMAL, Position(0, 0), Size(128, 16)), name(name) {}

    void onDraw()
    {
        Size bounds = gfx.getTextBounds(name);
        gfx.drawText((size.width - bounds.width) / 2, (size.height - bounds.height) / 2 + 1, name);
        //gfx.drawText(0, 8, name, Color::WHITE);
    }

    void onSelect()
    {
        drawBorder = true;
    }

    void onUnselect()
    {
        drawBorder = false;
    }

    void onNext()
    {

    }

    void onPrev()
    {

    }
};