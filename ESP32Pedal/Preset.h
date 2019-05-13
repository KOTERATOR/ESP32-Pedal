#pragma once

#include "EffectsUnit.h"

class Preset
{
private:
    String name = "Preset";
public:
    List<EffectsUnit*> effects;

    void proceed(int16_t * in);
};


void Preset::proceed(int16_t * in)
{
    int16_t out = in;
    for(int i = 0; i < effects.size(); i++)
    {
        effects[i]->proceed(&out);
    }
}