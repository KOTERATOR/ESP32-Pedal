#pragma once

#include "Dimensions.h"
#include "SSD1306.h"
#include "Adafruit_GFX.h"
#include "Container.h"

class Color
{
public:
    static const int TRANSPARENT = 3,
                     BLACK = 0,
                     WHITE = 1,
                     INVERSE = 2;
};

enum class HTextAlignment
{
    Left,
    Right,
    Center
};

enum class VTextAlignment
{
    Top,
    Bottom,
    Center
};

class ViewGFX : public Adafruit_GFX
{
protected:
    int8_t **buffer;
    Size size;
    Position offset;
    Container * currentContainer;

    static SSD1306 *display_ptr;

public:
    ViewGFX(Size size);
    ~ViewGFX();

    void setOffset(Position offset) { this->offset = offset; }

    static void setDisplay(SSD1306 *display);

    void draw();
    void clear();

    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawText(int16_t x, int16_t y, String &str, int16_t color = Color::WHITE, int8_t textSize = 1);
    void drawTextWithInvertedChar(int16_t x, int16_t y, String &str, int16_t color, int8_t textSize, size_t invertCharIndex, int16_t invertColor = Color::BLACK);
    void drawAlignedText(HTextAlignment hmode, VTextAlignment vmode, String &str, int16_t color, int8_t textSize = 1);
    Size getTextBounds(String & str, uint8_t textSize = 1);

    void setCurrentContainer(Container * c)
    {
        currentContainer = c;
    }

    Size getSize() { return size; }
    void setSize(Size size);

    int16_t width() { return size.width; }
    int16_t height() { return size.height; }
};

SSD1306 *ViewGFX::display_ptr;

ViewGFX::ViewGFX(Size size) : Adafruit_GFX(size.width, size.height)
{
    this->size = size;
    Serial.println("ViewGFX ctor");
    Serial.println(ESP.getFreeHeap());
    buffer = new int8_t *[size.width];
    for (int i = 0; i < size.width; i++)
    {
        buffer[i] = new int8_t[size.height];
        for (int j = 0; j < size.height; j++)
        {
            buffer[i][j] = Color::TRANSPARENT;
        }
    }
    cp437(true);
}

ViewGFX::~ViewGFX()
{
    Serial.println("ViewGFX dtor");
    Serial.print(size.width); Serial.print(" "); Serial.println(size.height);
    auto he = ESP.getFreeHeap();
    for (int i = 0; i < width(); i++)
    {
        delete[] (buffer[i]);
    }
    delete [] buffer;
    Serial.println("Deallocated - " + String(ESP.getFreeHeap()-he));
}

void ViewGFX::setDisplay(SSD1306 *display)
{
    ViewGFX::display_ptr = display;
}

void ViewGFX::draw()
{
    if (display_ptr != nullptr)
    {
        for (int x = 0; x < size.width; x++)
        {
            for (int y = 0; y < size.height; y++)
            {
                if (buffer[x][y] != Color::TRANSPARENT)
                {
                    OLEDDISPLAY_COLOR color;
                    if (buffer[x][y] == Color::BLACK)
                    {
                        color = BLACK;
                    }
                    else if (buffer[x][y] == Color::WHITE)
                    {
                        color = WHITE;
                    }
                    else if (buffer[x][y] == Color::INVERSE)
                    {
                        color = INVERSE;
                    }
                    display_ptr->setColor(color);
                    display_ptr->setPixel(offset.x + x, offset.y + y);
                }
            }
        }
    }
}

void ViewGFX::clear()
{
    for (int x = 0; x < size.width; x++)
    {
        for (int y = 0; y < size.height; y++)
        {
            buffer[x][y] = Color::TRANSPARENT;
        }
    }
}

void ViewGFX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if(currentContainer != nullptr)
    {
        Size ccSize = currentContainer->getSize();
        if (x >= 0 && x < ccSize.width && y >= 0 && y < ccSize.height)
        {
            Position ccPos = currentContainer->getAbsolutePosition();
            x += ccPos.x;
            y += ccPos.y;
            if (x >= 0 && x < this->size.width && y >= 0 && y < this->size.width)
            {
                switch (color)
                {
                case Color::BLACK:
                    buffer[x][y] = Color::BLACK;
                    break;
                case Color::WHITE:
                    buffer[x][y] = Color::WHITE;
                    break;
                case Color::INVERSE:
                    buffer[x][y] = Color::INVERSE;
                    break;
                case Color::TRANSPARENT:
                default:
                    buffer[x][y] = Color::TRANSPARENT;
                    break;
                }
            }
        }
    }
}

void ViewGFX::drawText(int16_t x, int16_t y, String &str, int16_t color, int8_t textSize)
{
    setCursor(x, y);
    setTextSize(textSize);
    setTextColor(color);

    for (int i = 0; i < str.length(); i++)
    {
        write(str[i]);
    }
}

void ViewGFX::drawTextWithInvertedChar(int16_t x, int16_t y, String &str, int16_t color, int8_t textSize, size_t invertCharIndex, int16_t invertColor)
{
    setCursor(x, y);
    setTextSize(textSize);
    setTextColor(color);
    uint16_t invertbgcolor = textcolor, invertcolor = invertColor;

    for (int i = 0; i < str.length(); i++)
    {
        if(i == invertCharIndex)
        {
            textcolor = invertColor;
            textbgcolor = invertbgcolor;
            write(str[i]);
            setTextColor(color);
        }
        else
        {
            write(str[i]);
        }
    }
}

void ViewGFX::drawAlignedText(HTextAlignment hmode, VTextAlignment vmode, String &str, int16_t color, int8_t textSize)
{
    int16_t x1 = 0, y1 = 0;
    Size bounds = getTextBounds(str, textSize);
    Size size = this->size;
    if(currentContainer != nullptr)
    {
        size = currentContainer->getSize();
    }
    if (hmode == HTextAlignment::Left)
    {
        x1 = 0;
    }
    else if (hmode == HTextAlignment::Right)
    {
        x1 = size.width - bounds.width;
    }
    else if (hmode == HTextAlignment::Center)
    {
        x1 = (size.width - bounds.width) / 2;
    }

    if (vmode == VTextAlignment::Top)
    {
        y1 = 0;
    }
    else if (vmode == VTextAlignment::Bottom)
    {
        y1 = size.height - bounds.height;
    }
    else if (vmode == VTextAlignment::Center)
    {
        y1 = (size.height - bounds.height) / 2;
    }
    drawText(x1, y1, str, color, textSize);
}

Size ViewGFX::getTextBounds(String & str, uint8_t textSize)
{
    int16_t x1, y1;
    uint16_t w, h;
    int len = str.length() + 1;
    char *text = new char[len];
    str.toCharArray(text, len);
    Adafruit_GFX::setTextSize(textSize);
    Adafruit_GFX::getTextBounds((char *)text, 0, 0, &x1, &y1, &w, &h);
    delete[] text;
    return Size(w, h);
}

void ViewGFX::setSize(Size size)
{
        for (int i = 0; i < width(); i++)
        {
            delete [] buffer[i];
        }

    this->size = size;

    buffer = new int8_t *[size.width];
    for (int i = 0; i < size.width; i++)
    {
        buffer[i] = new int8_t[size.height];
        for (int j = 0; j < size.height; j++)
        {
            buffer[i][j] = false;
        }
    }
}