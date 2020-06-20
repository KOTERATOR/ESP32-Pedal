#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"

class Chorus : public EffectsUnit
{
protected:
    const static uint16_t MIN_DELAY = 200, MAX_DELAY = 500;
    uint16_t sDelayBuffer0[MAX_DELAY+500] = {};
    uint16_t sDelayBuffer1[MAX_DELAY+500] = {};
    unsigned int DelayCounter = 0;
    unsigned int Delay_Depth = 300;
    unsigned int count_up = 1;
    int p = 0;

    PotentiometerView depth = PotentiometerView(this, "DEPTH", 2047);
    PotentiometerView volume = PotentiometerView(this, "VOLUME", 4095);
public:

    Chorus() : EffectsUnit("CHORUS")
    {
        
    }

    void processing(int16_t * in1, int16_t * in2)
    {
        sDelayBuffer0[DelayCounter] = (*in1);
        sDelayBuffer1[DelayCounter] = (*in2);
        int depthVal = map(depth.getValue(), 0, 4095, 1, 25);

        DelayCounter++;
        if(DelayCounter >= Delay_Depth) 
        {
            DelayCounter = 0; 
            if(count_up)
            {
                for(p=0;p<depthVal+1;p++) 
                    sDelayBuffer0[Delay_Depth+p]=sDelayBuffer0[Delay_Depth-1]; 

                Delay_Depth=Delay_Depth+depthVal;
                if (Delay_Depth>=MAX_DELAY)count_up = 0;
            }
            else
            {
                Delay_Depth=Delay_Depth-depthVal;
                if (Delay_Depth<=MIN_DELAY)count_up=1;
            }
        }
        float vol = volume.getRangedValue();
        (*in1) = ((int16_t)((float)sDelayBuffer0[DelayCounter]*vol) + (*in1)) >> 1;
        (*in2) = ((int16_t)((float)sDelayBuffer1[DelayCounter]*vol) + (*in2)) >> 1;
    }
};