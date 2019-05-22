#pragma once

#include "../GFX/View.h"
#include "../EffectsUnit.h"

class ControlView : public View
{
public:
    ControlView(EffectsUnit * effectsUnit) : View(ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, ContainerMode::NORMAL, Position(0, 0), Size(48, 48))
    {
        effectsUnit->controls.add(this);
    }
};

