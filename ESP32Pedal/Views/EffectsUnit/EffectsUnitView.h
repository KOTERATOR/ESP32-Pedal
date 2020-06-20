#pragma once

#include "../../Pedal.h"
#include "../../GFX/ClickView.h"
#include "../../Activities/EffectActivity.h"
#include "../../EffectsUnit.h"

class EffectsUnitView : public ClickView
{
public:
    EffectsUnit * unit;
    EffectsUnitView(EffectsUnit * unit) : ClickView(ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, ContainerMode::NORMAL, Position(0, 0), Size(48, 48)), unit(unit)
    {
        onSelectEvent([=](){ pedal.intent(new EffectActivity(unit)); });
    }

    void onDraw(ViewGFX * gfx)
    {
        gfx->drawAlignedText(HTextAlignment::Center, VTextAlignment::Center, unit->getName(), Color::WHITE);
    }
};

