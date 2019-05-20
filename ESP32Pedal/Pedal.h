#pragma once

#include "../ESP32Pedal/Hardware/Audio.h"
#include "../ESP32Pedal/Hardware/HardwareTimer.h"
#include "../ESP32Pedal/Utils/List.h"
#include "../ESP32Pedal/Hardware/Encoder.h"
#include "../ESP32Pedal/Hardware/Potentiometer.h"
#include "EffectsUnit.h"

class Pedal
{
private:
    Audio audio;
    int samplingRate = 0;
    int16_t in1, in2; 

    // LED-display
    //SSD1306 display(0x3c, 21, 22);

    // Controls
    //Encoder encoder(18, 19, 4);
    //Potentiometer potentiometer = Potentiometer(15);

public:
    List<EffectsUnit*> effects;
    Pedal(int samplingRate);
    void proceed();
    void getInput();
    void controlsTick();
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
    float volK = 1.0;//(float)potentiometer.getValue() / 4095.0;
    audio.output(((float)out1)*volK, ((float)out2*volK));
}

void Pedal::getInput()
{
    audio.getInput(&in1, &in2);
    //Serial.println(in);
}

void Pedal::controlsTick()
{
    //enc.tick();
    //potentiometer.tick();
}