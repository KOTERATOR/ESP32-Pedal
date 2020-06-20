#pragma once


#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"



class Delay : public EffectsUnit
{
private:
    const static uint16_t MAX_DELAY = 6250;
    int8_t delaybuffer1[MAX_DELAY] = {}, delaybuffer2[MAX_DELAY] = {};
    int counter = 0, delay_depth = 1;
    PotentiometerView delay = PotentiometerView(this, "DELAY", 2048);
    PotentiometerView feedback = PotentiometerView(this, "FEEDBACK", 2048);
    PotentiometerView level = PotentiometerView(this, "LEVEL", 2048);
public:

    Delay() : EffectsUnit("DELAY") 
    { 
        for(int i = 0; i < MAX_DELAY; i++)
        {
            delaybuffer1[i] = 0;
            delaybuffer2[i] = 0;
        }
    }

    void processing(int16_t * in1, int16_t * in2)
    {
        //Serial.println(in);
        float feedback_level = feedback.getRangedValue();
        if(feedback_level == 0.0)
        {
            delaybuffer1[counter] = (*in1)/16;
            delaybuffer2[counter] = (*in2)/16;
        }
        else
        {
            delaybuffer1[counter] = ((*in1)/16 + ((float)delaybuffer1[counter])*feedback_level);
            delaybuffer2[counter] = ((*in2)/16 + ((float)delaybuffer2[counter])*feedback_level);
        }
        delay_depth = map(delay.getValue(), 0, 4095, 1, MAX_DELAY-1);
        counter++;
        if(counter >= delay_depth)
        {
            counter = 0;
        }

        (*in1) = ((*in1)+(delaybuffer1[counter])*level.getRangedValue()*16)/2;
        (*in2) = ((*in2)+(delaybuffer2[counter])*level.getRangedValue()*16)/2;
    }

    ~Delay()
    {
    }
};
