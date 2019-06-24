#pragma once

class VirtualPotentiometer
{
private:
    int value;
public:
    int getValue();
    float getRangedValue();
    void setValue(int value);

    virtual void tick() {}
};

int VirtualPotentiometer::getValue()
{
    return value;
}

void VirtualPotentiometer::setValue(int value)
{
    if(value > 4095)
    {
        value = 4095;
    }
    else if(value < 0)
    {
        value = 0;
    }
    this->value = value;
}

float VirtualPotentiometer::getRangedValue()
{
    return ((float)value)/4095.0;
}