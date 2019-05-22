#pragma once

#include "../GFX/Activity.h"
#include "../../ESP32Pedal/EffectsUnit.h"
#include "../Views/Header.h"
#include "../GFX/Layout.h"

class EffectActivity : public Activity
{
private:
    EffectsUnit * unit;
    Header effectHeader;
    Layout controlsLayout = Layout(LayoutMode::HORIZONTAL, ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0), Size(128, 48));
public:
    EffectActivity(EffectsUnit * unit) : Activity(), effectHeader(unit->name)
    {
        this->unit = unit;
        add(effectHeader);
        for(int i = 0; i < unit->controls.size(); i++)
        {
            controlsLayout.add(*((Container*)unit->controls[i]));
        }
        add(controlsLayout);
    }
};