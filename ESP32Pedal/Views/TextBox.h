#pragma once

#include "../GFX/View.h"

class TextBox : public View
{
public:
    String alphabet = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
    size_t currentPos = 0, currentAlphabetChar = 0;
    char prevChar = ' ';
    String text;
    bool editMode = false;
    int8_t textSize = 1;
    int16_t color;
    HTextAlignment horizontalAlignment = HTextAlignment::Left;
    VTextAlignment verticalAlignment = VTextAlignment::Top;
    TextBox(ContainerSizeMode widthMode, ContainerSizeMode heightMode, String text = "",  int textSize = 1, int16_t color = Color::WHITE, HTextAlignment horizontalAlignment = HTextAlignment::Center, VTextAlignment verticalAlignment = VTextAlignment::Center) : View(widthMode, heightMode, ContainerMode::NORMAL, Position(0,0), Size(128, 10)), text(text)
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
        if(isSelected)
        {
            if(editMode)
            {
                text[currentPos] = alphabet[currentAlphabetChar];
            }
            gfx->drawTextWithInvertedChar(x1, y1, text, color, textSize, currentPos);
        }
        else
        {
            gfx->drawText(x1, y1, text, color, textSize);
        }
        //Size bounds = gfx.getTextBounds(text);
        //gfx.drawText((size.width - bounds.width) / 2, (size.height - bounds.height) / 2 + 1, text, Color::WHITE);
        //gfx.drawText(0, 10, text, color, textSize);
    }

    void onSelect()
    {
        if(isSelected && !editMode)
        {
            editMode = true;
            currentAlphabetChar = 0;
            prevChar = text[currentPos];
        }
        if(isSelected && editMode)
        {
            editMode = false;
        }
        isSelected = true;
    }

    void onNext()
    {
        if(editMode)
        {
            if(currentAlphabetChar + 1 < alphabet.length())
                currentAlphabetChar++;
            else
            {
                currentAlphabetChar = 0;
            }
        }
        else
        {
            if(currentPos + 1 <= text.length())
                currentPos++;
        }
    }

    void onPrev()
    {
        if(editMode)
        {
            if(currentAlphabetChar > 0)
                currentAlphabetChar--;
            else
                currentAlphabetChar = alphabet.length()-1;
        }
        else
        {
            if(currentPos > 0)
                currentPos--;
        }
    }

    bool onUnselect()
    {
        if(editMode)
        {
            editMode = false;
            text[currentPos] = prevChar;
            return false;
        }
        else
        {
            isSelected = false;
            return true;
        }
    }
};