#pragma once

#include "../GFX/View.h"
#include "../Utils/Event.h"

class ProgressBar : public View
{
private:
    float value = 0;

public:
    Event<float> onValueChanged;
    ProgressBar(ContainerSizeMode widthMode, ContainerSizeMode heightMode, bool horizontal = true, bool displayPercentage = false, float value = 0, ContainerMode mode = ContainerMode::NORMAL) : View(widthMode, heightMode, mode)
    {

    }

    void setValue(float value) 
    { 
        if(value > 100.0)
        {
            value = 100.0;
        }
        else if (value < 0.0)
        {
            value = 0.0;
        }
        this->value = value;
        onValueChanged.invoke(this->value);
    }
    float getValue() { return value; }

    void onDraw()
    {
        //gfx.fillRect(0, 0, size.width, size.height, Color::BLACK);
        gfx.drawRect(0, 0, size.width, size.height, Color::WHITE);
        int pixels = (size.width * value)/100.0;
        for(int i = 0; i < pixels; i++)
        {
            gfx.drawLine(i, 0, i, size.height, Color::WHITE);
        }
        //gfx.drawAlignedText(HTextAlignment::Left, VTextAlignment::Top, (String(value)+"%"), Color::WHITE, 2);
    }
};