#pragma once

#include "../Utils/Event.h"
#include "Button.h"

class Encoder
{
private:
    long prevMillis = 0;
    int pinA = 0, pinB = 0, pinD = 0;
    bool A = false, B = false;
    
public:
    Encoder(int pinA, int pinB, int pinD);
    Button button;
    Event<> onCW, onCCW;
    void tick();
};

Encoder::Encoder(int pinA, int pinB, int pinD) : button(pinD)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinD = pinD;
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
}

void Encoder::tick()
{
    
    long now = millis();
    if(now - prevMillis >= 1)
    {
        bool newA = digitalRead(pinA), newB = digitalRead(pinB);
        if(B && !newB && !newA)
        {
            onCW();
        }
        else if(A && !newA && !B)
        {
            onCCW();
        }
        
        A = newA;
        B = newB;
        prevMillis = now;
    }
    button.tick();
}