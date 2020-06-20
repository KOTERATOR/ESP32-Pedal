#pragma once

#include "Utils/List.h"

class ControlView;
class Preset;

class EffectsUnit
{
protected:
    Preset * preset;
public:
    List<ControlView*> controls;

    bool bypass = false;
    String name;

    virtual void processing(int16_t * out1, int16_t * out2) = 0;

    void proceed(int16_t * out1, int16_t * out2);

    String & getName()
    {
        return name;
    }

    Preset * getPreset()
    {
        return preset;
    }

    void setPreset(Preset * preset)
    {
        this->preset = preset;
    }

    EffectsUnit(String name, Preset * preset = nullptr) : preset(preset) 
    {
        this->name = name;
    }
};

void EffectsUnit::proceed(int16_t * out1, int16_t * out2)
{
    if(!bypass)
    {
        processing(out1, out2);
    }
}