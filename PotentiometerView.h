#pragma once

#include "libs/View.h"
#include "VirtualPotentiometer.h"

class PotentiometerView : public View
{
public:
    VirtualPotentiometer & pot;
    String name;
    PotentiometerView(VirtualPotentiometer & pot, String name) : View(ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, ContainerMode::NORMAL, Position(0, 0), Size(32, 48)), pot(pot), name(name) {}

    void onDraw()
    {
        int r = (size.width-2)/2;
        int x, y;
        x = y = size.width/2;
        gfx.drawCircle(x, y, r, Color::WHITE);
        int angle = -map(pot.getValue(), 0, 4095, 0, 270) - 135;
        float rad = radians(angle-90);
        int x1 = (x - (sin(rad) * r));
        int y1 = (y - (cos(rad) * r));
        gfx.drawLine(x, y, x1, y1, Color::WHITE);
        Size bounds = gfx.getTextBounds(name);
        gfx.drawText((size.width - bounds.width)/2, size.height - bounds.height, name);
    }
};