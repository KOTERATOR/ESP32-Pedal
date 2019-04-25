#pragma once

#include "../ESP32Pedal/Hardware/Audio.h"
#include "../ESP32Pedal/Hardware/HardwareTimer.h"
#include "../ESP32Pedal/Utils/List.h"
#include "EffectsUnit.h"

class Pedal
{
private:
    Audio audio;
    int sampling_rate = 0;
    List<EffectsUnit*> effects;

    int16_t in1 = 0, in2 = 0; 
public:
    Pedal(int samplingRate);
    void proceed();
    void getInput();
};

Pedal * pedal = nullptr;

void onTick(void*)
{
    TIMERG0.int_clr_timers.t0 = 1;
    TIMERG0.hw_timer[0].config.alarm_en = 1;
    pedal->proceed();
}

Pedal::Pedal(int samplingRate)
{

}

void Pedal::proceed()
{
    int16_t out1 = in1, out2 = in2;
    for(int i = 0; i < effects.size(); i++)
    {
        effects[i]->proceed(&out1, &out2);
    }
    audio.output(out1, out2);
}

void Pedal::getInput()
{
    audio.getInput(&in1, &in2);
}