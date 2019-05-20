#pragma once

#include "Dimensions.h"
#include "SSD1306.h"
#include "Adafruit_GFX.h"

class Color
{
public:
    static const int TRANSPARENT = 3,
    BLACK = 0,
    WHITE = 1,
    INVERSE = 2;
};

class ViewGFX : public Adafruit_GFX
{
  protected:
    int8_t **buffer;
    Size size;
    Position offset;

    static SSD1306 *display_ptr;

  public:
    ViewGFX(Size size);
    ~ViewGFX();

    void setOffset(Position offset) { this->offset = offset; }

    static void setDisplay(SSD1306 *display);

    void draw();
    void clear();

    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawText(int16_t x, int16_t y, String str, int16_t color = Color::WHITE, int8_t textSize = 1);
    Size getTextBounds(String str);

    Size getSize() { return size; }
    void setSize(Size size);

    int16_t width() { return size.width; }
    int16_t height() { return size.height; }
};

SSD1306 *ViewGFX::display_ptr;

ViewGFX::ViewGFX(Size size) : Adafruit_GFX(size.width, size.height)
{
    this->size = size;
    buffer = new int8_t *[size.width];
    for (int i = 0; i < size.width; i++)
    {
        buffer[i] = new int8_t[size.height];
        for (int j = 0; j < size.height; j++)
        {
            buffer[i][j] = Color::BLACK;
        }
    }
    cp437(true);
}

ViewGFX::~ViewGFX()
{
    for (int i = 0; i < width(); i++)
    {
        delete[] buffer[i];
    }
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
                    if(buffer[x][y] == Color::BLACK)
                    {
                        color = BLACK;
                    }
                    else if(buffer[x][y] == Color::WHITE)
                    {
                        color = WHITE;
                    }
                    else if(buffer[x][y] == Color::INVERSE)
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
    if (x >= 0 && x < this->width() && y >= 0 && y < this->height())
    {
        switch(color)
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

void ViewGFX::drawText(int16_t x, int16_t y, String str, int16_t color, int8_t textSize)
{
    setCursor(x, y);
    setTextSize(textSize);
    setTextColor(color);
    
    for(int i = 0; i < str.length(); i++)
    {
        write(str[i]);
    }
}

Size ViewGFX::getTextBounds(String str)
{
    int16_t x1, y1;
    uint16_t w, h;
    int len = str.length()+1;
    char * text = new char[len];
    str.toCharArray(text, len);
    Adafruit_GFX::getTextBounds((char*)text, 0, 0, &x1, &y1, &w, &h);
    delete [] text;
    return Size(w, h);
}

void ViewGFX::setSize(Size size)
{
    for (int i = 0; i < width(); i++)
    {
        delete[] buffer[i];
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