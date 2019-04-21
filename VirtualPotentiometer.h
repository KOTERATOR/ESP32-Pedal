#pragma once

class VirtualPotentiometer
{
private:
    int value;

public:
    int getValue();
    void setValue(int value);

    virtual void read() {}
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