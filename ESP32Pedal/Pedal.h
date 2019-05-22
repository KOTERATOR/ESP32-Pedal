#pragma once

#include "Hardware/Audio.h"
#include "Hardware/HardwareTimer.h"
#include "Utils/List.h"
#include "Hardware/Encoder.h"
#include "Hardware/Potentiometer.h"
#include "GFX/Screen.h"
#include "EffectsUnit.h"
#include "Activities/EffectActivity.h"
#include "Effects/Fuzz.h"
#include "GFX/Screen.h"

class Pedal
{
private:
    Audio audio;
    
    Fuzz fuzz = Fuzz();
    EffectActivity mainActivity = EffectActivity(&fuzz);

    int samplingRate = 0;
    int16_t in1, in2; 

    // LED-display
    

    // Controls
    Encoder encoder = Encoder(18, 19, 4);
    Potentiometer potentiometer = Potentiometer(15);

public:
    List<EffectsUnit*> effects;
    Pedal(int samplingRate);
    void proceed();
    void getInput();
    void controlsTick();

    void draw();
};

Pedal::Pedal(int samplingRate = 12000)
{
    this->samplingRate = samplingRate;
    

    encoder.onCW += [](){ onNext(); };
    encoder.onCCW += [](){ onPrev(); };
    encoder.button.onOnceClick += [](){ onSelect(); };
    encoder.button.onHold += [](){ onUnselect(); };

    effects.add(&fuzz);

    screen.setActivity(&mainActivity);
}

void Pedal::proceed()
{
    int16_t out1 = in1, out2 = in2;
    for(int i = 0; i < effects.size(); i++)
    {
        effects[i]->proceed(&out1, &out2);
    }
    //float volK = 1.0;
    float volK = (float)potentiometer.getValue() / 4095.0;
    audio.output(((float)out1)*volK, ((float)out2*volK));
}

void Pedal::getInput()
{
    audio.getInput(&in1, &in2);
    //Serial.println(in);
}

void Pedal::controlsTick()
{
    encoder.tick();
    potentiometer.tick();
}

void Pedal::draw()
{
    screen.draw();
}