#pragma once

#include "../GFX/Activity.h"
#include "../Preset.h"
#include "../Views/ClickHeader.h"

class EffectSettingsActivity : public Activity
{
protected:
    EffectsUnit * unit;
public:
    ClickHeader bypassEffect = ClickHeader("BYPASS FX"), removeEffect = ClickHeader("REMOVE FX");

    EffectSettingsActivity(EffectsUnit * unit) : Activity()
    {
        this->unit = unit;

        bypassEffect.onSelectEvent([=]()
        {
            unit->bypass = !unit->bypass;
            pedal.exitActivity();
        });

        removeEffect.onSelectEvent([=]()
        {
            pedal.setActivity(pedal.getCurrentActivity()->intentActivity->intentActivity);
            unit->getPreset()->remove(unit);
        });

        add(bypassEffect);
        add(removeEffect);
    }
};