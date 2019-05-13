#pragma once

#include "../ESP32Pedal/Hardware/Audio.h"
#include "../ESP32Pedal/Hardware/HardwareTimer.h"
#include "../ESP32Pedal/Utils/List.h"
#include "../ESP32Pedal/Hardware/Encoder.h"
#include "EffectsUnit.h"

class Pedal
{
private:
    Audio audio;
    int samplingRate = 0;
    

    int16_t in1, in2; 
public:
    List<EffectsUnit*> effects;
    Pedal(int samplingRate);
    void proceed();
    void getInput();
};

Pedal::Pedal(int samplingRate = 12000)
{
    this->samplingRate = samplingRate;
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
    //Serial.println(in);
}