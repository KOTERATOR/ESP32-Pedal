#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"

#define BUFFERSIZE 1024

class Octaver : public EffectsUnit
{
private:
    int16_t inBufA[BUFFERSIZE] = {}, inBufB[BUFFERSIZE] = {};
    int16_t inBuf2A[BUFFERSIZE] = {}, inBuf2B[BUFFERSIZE] = {};
    //int16_t outBuf[1024] = {};
    int counter = 0;
    float volK = 1.0;
    bool dir = true;
    int16_t value = 0;
    PotentiometerView pitch = PotentiometerView(this, "PITCH", 2047);
public:

    Octaver() : EffectsUnit("OCTAVER")
    {
        for(int i = 0; i < BUFFERSIZE; i++)
        {
            inBufA[i] = inBufB[i] = inBuf2A[i] = inBuf2B[i] = 0;
        }
    }

    void processing(int16_t * in1, int16_t * in2)
    {
        counter++;
        if (counter >= BUFFERSIZE)
        {
            counter = 0;
            
        }
    }
};