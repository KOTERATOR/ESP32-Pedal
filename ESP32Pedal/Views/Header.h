#pragma once

#include "../GFX/EventView.h"
#include "../Utils/Event.h"

class Header : public EventView
{
private:
    String name;

public:
    Header(String name) : EventView(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::FIXED, ContainerMode::NORMAL, Position(0, 0), Size(128, 16)), name(name) {}

    void onDraw(ViewGFX *gfx)
    {
        Size bounds = gfx->getTextBounds(name);
        gfx->drawText((size.width - bounds.width) / 2, (size.height - bounds.height) / 2 + 1, name);
        //gfx.drawText(0, 8, name, Color::WHITE);
    }

    void setText(String &text)
    {
        name = text;
    }
};