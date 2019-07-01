#pragma once

#include "EffectsUnit.h"

class Preset
{
private:
    
public:
    String name = "Preset";
    List<EffectsUnit*> effects;

    Preset(String name)
    {
        this->name = name;
    }

    void add(EffectsUnit * unit)
    {
        effects.add(unit);
    }

    void remove(EffectsUnit * unit)
    {
        effects.remove(unit);
    }

    void proceed(int16_t * in1, int16_t * in2);
};


void Preset::proceed(int16_t * in1, int16_t * in2)
{
    int16_t out1 = (*in1), out2 = (*in2);
    for(int i = 0; i < effects.size(); i++)
    {
        effects[i]->proceed(&out1, &out2);
    }
    (*in1) = out1;
    (*in2) = out2;
}