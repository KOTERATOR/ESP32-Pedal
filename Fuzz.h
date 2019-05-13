#pragma once

#include "ESP32Pedal/EffectsUnit.h"

class Fuzz : public EffectsUnit
{
public:
    void processing(int16_t * in1, int16_t * in2)
    {
        (*in1) *= 10;
        (*in2) *= 10;
    }
};