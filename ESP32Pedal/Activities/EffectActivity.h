#pragma once

#include "../GFX/Activity.h"
#include "../Views/EffectsUnit/EffectsUnitHeader.h"
#include "../GFX/Layout.h"
#include "../Pedal.h"
#include "../EffectsUnit.h"
#include "EffectSettingsActivity.h"

class EffectActivity : public Activity
{
private:
    EffectsUnit * unit;
    EffectsUnitHeader effectHeader;
    Layout controlsLayout = Layout(LayoutMode::HORIZONTAL, ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0), Size(128, 48), ContainerMode::CENTER);
public:
    EffectActivity(EffectsUnit * unit) : Activity(), effectHeader(unit)
    {
        this->unit = unit;
        add(effectHeader);
        for(int i = 0; i < unit->controls.size(); i++)
        {
            controlsLayout.add(*((Container*)unit->controls[i]));
        }
        add(controlsLayout);

        effectHeader.onSelectEvent([=]()
        {
            pedal.intent(new EffectSettingsActivity(unit));
        });
    }

    bool onExit()
    {
        //pedal.onStartup.invoke();
        return true;
    }
};