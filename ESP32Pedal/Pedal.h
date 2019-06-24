#pragma once

#include "Hardware/Audio.h"
#include "Hardware/HardwareTimer.h"
#include "Utils/List.h"
#include "Hardware/Encoder.h"
#include "Hardware/Potentiometer.h"
#include "GFX/Screen.h"
#include "EffectsUnit.h"
#include "Activities/EffectActivity.h"
#include "Activities/UpdateActivity.h"
#include "Effects/Fuzz.h"
#include "Effects/Delay.h"
#include "Effects/Octaver.h"
#include "GFX/Screen.h"

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void timerTick();

volatile bool canOutput = false;

class Pedal
{
private:
    hw_timer_t * timer = NULL;
    Audio audio;
    
    Octaver delay_effect = Octaver();
    EffectActivity mainActivity = EffectActivity(&delay_effect);

    static int samplingRate;
    int16_t in1 = 0, in2 = 0; 

    // Controls
    Encoder encoder = Encoder(18, 19, 4);
    Potentiometer potentiometer = Potentiometer(15);
    Button footswitch = Button(12, INPUT_PULLUP);

    // OTA
    bool otaEnabled = true;


    // WiFi (todo : replace with settings)


public:
    List<EffectsUnit*> effects;
    Pedal();

    void proceed();
    void getInput();
    void controlsTick();
    void startup();
    
    void draw();

    static int getSamplingRate()
    {
        return samplingRate;
    }

    void setActivity(Activity * activity)
    {
        screen.setActivity(activity);
    }

    void setLED(int value)
    {
        analogWrite(14, value);
    }

} pedal;

// костыль
//#include "Utils/PedalOTA.h"

int Pedal::samplingRate = 25000;

Pedal::Pedal()
{   
    encoder.onCW += [](){ onNext(); };
    encoder.onCCW += [](){ onPrev(); };
    encoder.button.onOnceClick += [](){ onSelect(); };
    encoder.button.onHold += [](){ onUnselect(); };

    pinMode(14, OUTPUT);

    analogWrite(14, 255);

    timer = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
    timerAttachInterrupt(timer, &timerTick, true); // edge (not level) triggered 
    timerAlarmWrite(timer, 1000000/samplingRate, true); // 1000000 * 1 us = 1 s, autoreload true
    timerAlarmEnable(timer);
}

void Pedal::proceed()
{
    if(canOutput)
    {
        int16_t out1 = in1, out2 = in2;
        for(int i = 0; i < effects.size(); i++)
        {
            effects[i]->proceed(&out1, &out2);
            if(out1 > 4095)
            {
                out1 = 4095;
            }
            else if(out1 < 0)
            {
                out1 = 0;
            }

            if(out2 > 4095)
            {
                out2 = 4095;
            }
            else if(out2 < 0)
            {
                out2 = 0;
            }
        }
        //float volK = 1.0;
        float volK = ((float)potentiometer.getValue()) / 4095.0;
        /*if(out > 0)
        {
            audio.output(((float)out)*volK, 0);
        }
        else if(out < 0)
        {
            audio.output(0, ((float)out)*volK);
        }
        else
        {
            audio.output(0, 0);
        }*/
        audio.output(((float)out1)*volK, ((float)out2)*volK);
        // reset watchdog
        timerWrite(timer, 0);
        canOutput = false;
    }
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
    footswitch.tick();
    /*if(ota != nullptr && otaEnabled)
    {
        ota->handle();
    }*/
}

void Pedal::startup()
{
    effects.add(&delay_effect);

    screen.setActivity(&mainActivity);
}

void Pedal::draw()
{
    screen.draw();
}

void IRAM_ATTR timerTick()
{
    portENTER_CRITICAL_ISR(&timerMux);
    canOutput = true;
    portEXIT_CRITICAL_ISR(&timerMux);
}

