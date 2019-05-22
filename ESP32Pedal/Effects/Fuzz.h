#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"

class Fuzz : public EffectsUnit
{
private:
    PotentiometerView gain = PotentiometerView(this, "GAIN");
public:
    Fuzz() : EffectsUnit("FUZZ") {}
    void processing(int16_t * in1, int16_t * in2)
    {
        (*in1) *= gain.getValue()/10;
        (*in2) *= gain.getValue()/10;
    }
};