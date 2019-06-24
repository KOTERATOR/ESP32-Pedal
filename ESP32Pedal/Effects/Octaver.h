#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"

#define BUFFERSIZE 512

class Octaver : public EffectsUnit
{
private:
    int16_t inBuf1A[BUFFERSIZE] = {}, inBuf1B[BUFFERSIZE] = {};
    int16_t inBuf2A[BUFFERSIZE] = {}, inBuf2B[BUFFERSIZE] = {};
    //int16_t outBuf[1024] = {};
    int writecounter = 0, c1 = 0, c2 = BUFFERSIZE/2, divider = 0;
    float volK = 1.0;
    bool dir = true;
    int16_t value = 0;
    PotentiometerView pitch = PotentiometerView(this, "PITCH", 2047);
public:
    Octaver() : EffectsUnit("OCTAVER")
    {
        for(int i = 0; i < BUFFERSIZE; i++)
        {
            inBuf1A[i] = inBuf1B[i] = inBuf2A[i] = inBuf2B[i] = 0;
        }
    }

    void processing(int16_t * in1, int16_t * in2)
    {
        //float val = pitch.getRangedValue()*BUFFERSIZE;
        inBuf1A[writecounter] = inBuf1B[writecounter] = (*in1);
        inBuf2A[writecounter] = inBuf2B[writecounter] = (*in2);
        writecounter++;
        if(writecounter >= BUFFERSIZE)
        {
            writecounter = 0;
        }
        float reverseVolK = (1.0 - volK);
        (*in1) = inBuf1A[c1]*volK + inBuf1B[c2]*reverseVolK;
        (*in2) = inBuf2A[c1]*volK + inBuf2B[c2]*reverseVolK;

        divider++;
        if (divider>=1)
        {
            c1 = c1 + 1;
            c2 = c2 + 1;
            divider=0;
        }

        if(c1 >= volK * BUFFERSIZE) c1 = 0; 
        if(c2 >= reverseVolK * BUFFERSIZE) c2 = 0;
        if(volK >= 1.0)
        {
            dir = false;
        }
        else if(volK <= 0.0)
        {
            dir = true;
        }

        float step = 1.0 / BUFFERSIZE;

        volK += (dir ? step : -step);
    }
};