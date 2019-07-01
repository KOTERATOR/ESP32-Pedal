#pragma once

#include "../GFX/View.h"

class Label : public View
{
public:
    String text;
    int8_t textSize = 1;
    int16_t color;
    HTextAlignment horizontalAlignment = HTextAlignment::Left;
    VTextAlignment verticalAlignment = VTextAlignment::Top;
    Label(ContainerSizeMode widthMode, ContainerSizeMode heightMode, String text = "",  int textSize = 1, int16_t color = Color::WHITE, HTextAlignment horizontalAlignment = HTextAlignment::Left, VTextAlignment verticalAlignment = VTextAlignment::Top) : View(widthMode, heightMode, ContainerMode::NORMAL, Position(0,0), Size(128, 10)), text(text)
    {
        this->textSize = textSize;
        this->horizontalAlignment = horizontalAlignment;
        this->verticalAlignment = verticalAlignment;
        this->color = color;
    }

    void onDraw(ViewGFX * gfx)
    {
        int16_t x1 = 0, y1 = 0;
        Size bounds = gfx->getTextBounds(text, textSize);
        if(horizontalAlignment == HTextAlignment::Left)
        {
            x1 = 0;
        }
        else if(horizontalAlignment == HTextAlignment::Right)
        {
            x1 = size.width - bounds.width;
        }
        else if(horizontalAlignment == HTextAlignment::Center)
        {
            x1 = (size.width - bounds.width)/2;
        }
        
        if(verticalAlignment == VTextAlignment::Top)
        {
            y1 = 0;
        }
        else if(verticalAlignment == VTextAlignment::Bottom)
        {
            y1 = size.height - bounds.height;
        }
        else if(verticalAlignment == VTextAlignment::Center)
        {
            y1 = (size.height - bounds.height)/2;
        }
        gfx->drawText(x1, y1, text, color, textSize);
        //Size bounds = gfx.getTextBounds(text);
        //gfx.drawText((size.width - bounds.width) / 2, (size.height - bounds.height) / 2 + 1, text, Color::WHITE);
        //gfx.drawText(0, 10, text, color, textSize);
    }
};