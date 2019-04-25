#pragma once

#include "ESP32Pedal/GFX/View.h"

class ControlView : public View
{
private:

public:
    virtual void onNext() = 0;
    virtual void onPrev() = 0;
};

