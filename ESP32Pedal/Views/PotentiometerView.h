#pragma once

#include "../Views/ControlView.h"
#include "../Controls/VirtualPotentiometer.h"

class PotentiometerView : public ControlView, public VirtualPotentiometer
{
public:
    String name;
    PotentiometerView(EffectsUnit * unit, String name, int value = 0) : ControlView(unit), name(name)
    {
        setValue(value);
    }

    void onDraw(ViewGFX * gfx)
    {
        int r = (32)/2;
        int x, y = 18;
        x = size.width/2;
        gfx->drawCircle(x, y, r, Color::WHITE);
        int angle = -map(getValue(), 0, 4095, 0, 270) - 135;
        float rad = radians(angle-90);
        int x1 = (x - (sin(rad) * r));
        int y1 = (y - (cos(rad) * r));
        gfx->drawLine(x, y, x1, y1, Color::WHITE);
        Size bounds = gfx->getTextBounds(name);
        gfx->drawText((size.width - bounds.width)/2, size.height - bounds.height-2, name);
    }

    void onNext()
    {
        setValue(getValue()+100);
    }

    void onPrev()
    {
        setValue(getValue()-100);
    }
};