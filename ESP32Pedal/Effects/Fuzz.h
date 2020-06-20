#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"

class Fuzz : public EffectsUnit
{
private:
    PotentiometerView gain = PotentiometerView(this, "GAIN");
public:

    Fuzz() : EffectsUnit("FUZZ") {}
    void processing(int16_t * out1, int16_t * out2)
    {
        int gainK = 1 + gain.getValue()/100;
        (*out1) *= gainK;
        (*out2) *= gainK;
    }
};