#pragma once

#include "Views/ControlView.h"
#include "Utils/List.h"

class EffectsUnit
{
public:
    List<ControlView*> controls;

    EffectsUnit() {}

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