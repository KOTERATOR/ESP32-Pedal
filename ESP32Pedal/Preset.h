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
        unit->setPreset(this);
    }

    void remove(EffectsUnit * unit)
    {
        if(unit != nullptr)
        {
            effects.remove(unit);
            delete unit;
        }
    }

    void proceed(int16_t * in1, int16_t * in2);
};


void Preset::proceed(int16_t * in1, int16_t * in2)
{
    int16_t out1 = (*in1), out2 = (*in2);
    for(int i = 0; i < effects.size(); i++)
    {
        effects[i]->proceed(&out1, &out2);
        if(out1 > 4095)
        {
            out1 = 4095;
        }
        else if(out1 < 0)
        {
            out1 = 0;
        }

        if(out2 > 4095)
        {
            out2 = 4095;
        }
        else if(out2 < 0)
        {
            out2 = 0;
        }
    }
    (*in1) = out1;
    (*in2) = out2;
}