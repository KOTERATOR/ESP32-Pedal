#pragma once

#include "../Pedal.h"
#include "../GFX/View.h"
#include "../Activities/EffectActivity.h"
#include "../EffectsUnit.h"

class EffectsUnitView : public View
{
public:
    String name;
    EffectsUnit * unit;
    EffectsUnitView(EffectsUnit * unit) : View(ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, ContainerMode::NORMAL, Position(0, 0), Size(48, 48)), unit(unit), name(unit->name)
    {

    }

    void onDraw(ViewGFX * gfx)
    {
        gfx->drawAlignedText(HTextAlignment::Center, VTextAlignment::Center, name, Color::WHITE);
    }

    void onSelect()
    {
        pedal.intent(new EffectActivity(unit));
    }
};

