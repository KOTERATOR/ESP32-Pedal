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
    this->value = value;
}