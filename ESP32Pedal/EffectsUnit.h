#pragma once

#include "Utils/List.h"

class ControlView;

class EffectsUnit
{
public:
    List<ControlView*> controls;
    String name;

    EffectsUnit(String name) : name(name) {}

    bool bypass = false;

    virtual void processing(int16_t * out1, int16_t * out2) = 0;

    void proceed(int16_t * out, int16_t * out2);
};

void EffectsUnit::proceed(int16_t * out1, int16_t * out2)
{
    if(!bypass)
    {
        processing(out1, out2);
    }
}