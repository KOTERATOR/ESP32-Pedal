#pragma once

class EffectsUnit
{
public:
    EffectsUnit();

    virtual void proceed(int16_t * out1, int16_t * out2) = 0;
};