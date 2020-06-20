#pragma once

#include "../ClickHeader.h"
#include "../../EffectsUnit.h"

class EffectsUnitHeader : public ClickHeader
{
    protected:
    bool bypass = false;
    EffectsUnit * unit;
    public:
    EffectsUnitHeader(EffectsUnit * unit) : unit(unit), ClickHeader(unit->getName())
    {
        bypass = unit->bypass;
    }

    void onDraw(ViewGFX * gfx)
    {
        if(bypass != unit->bypass)
        {
            setText(unit->getName() + (unit->bypass ? " (BYPASS)" : ""));
            bypass = unit->bypass;
        }
        ClickHeader::onDraw(gfx);
    }
};