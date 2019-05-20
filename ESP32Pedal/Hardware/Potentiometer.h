#pragma once

#include "../Controls/VirtualPotentiometer.h"

class Potentiometer : public VirtualPotentiometer
{
protected:
    int pin = 0;
public:
    Potentiometer(int pin)
    {
        this->pin = pin;
    }
    
    void tick()
    {
        setValue(analogRead(pin));
    }
};