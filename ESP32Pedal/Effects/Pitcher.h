#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"
#include "pitchshifter.h"

#define BUFFERSIZE 512

class Pitcher : public EffectsUnit
{
private:
    float inBuf[BUFFERSIZE] = {}, psBuf[BUFFERSIZE] = {};
    //int16_t outBuf[1024] = {};
    int counter = 0;
    int16_t value = 0;
    PotentiometerView pitch = PotentiometerView(this, "PITCH", 2047);
public:
    Pitcher() : EffectsUnit("PITCHER")
    {
        for(int i = 0; i < BUFFERSIZE; i++)
        {
            inBuf[i] = 0;
            psBuf[i] = 0;
            //outBuf[i] = 0;
        }
    }

    void processing(int16_t * in1, int16_t * in2)
    {
        inBuf[counter] = (*in1) - (*in2);
        
        counter++;
        if(counter >= BUFFERSIZE)
        {
            counter = 0;
            //PitchShifter::PitchShift(pitch.getRangedValue(), 25000.0, inBuf, 1024);
            //int cur = micros();
            //smbFft(inBuf, BUFFERSIZE, -1);
            smbPitchShift(pitch.getRangedValue() * 2.0f, BUFFERSIZE, 256, 1, 25000, inBuf, psBuf);
            /* for(int i = 0; i < BUFFERSIZE; i++)
            {
                psBuf[i] = inBuf[i/2];
                
            }*/
            //Serial.println(micros() - cur);
        }
        int16_t outVal = (int16_t)psBuf[counter];
        
        if(outVal >= 0)
        {
            (*in1) = outVal;
            (*in2) = 0;
        }
        else
        {
            (*in1) = 0;
            (*in2) = -outVal;
        }
        
    }
};