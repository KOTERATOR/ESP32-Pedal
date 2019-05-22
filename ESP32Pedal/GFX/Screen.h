#pragma once

#include "Activity.h"
#include "ViewGFX.h"
#include "SSD1306.h"

class Screen
{
private:
    Activity * currentActivity = nullptr;
    SSD1306 * display_ptr = nullptr;
public:
    Screen() 
    {   

    }

    void setDisplay(SSD1306 * display)
    {
        this->display_ptr = display;
        ViewGFX::setDisplay(display_ptr);
    }

    void setActivity(Activity * activity)
    {
        currentActivity = activity;
        currentActivity->calculate();
    }
    void draw()
    {
        display_ptr->clear();
        if(currentActivity != nullptr)
        {
            currentActivity->draw();
        }
        display_ptr->display();
    }

    void onNext()
    {
        if(currentActivity != nullptr)
        {
            currentActivity->onNext();
        }
    }

    void onPrev()
    {
        if(currentActivity != nullptr)
        {
            currentActivity->onPrev();
        }
    }

    void onSelect()
    {
        if(currentActivity != nullptr)
        {
            currentActivity->onSelect();
        }
    }

    void onUnselect()
    {
        if(currentActivity != nullptr)
        {
            if(currentActivity->onUnselect())
            {
                currentActivity->onExit();
            }
        }
    }
} screen;

void onNext()
{
    screen.onNext();
}

void onPrev()
{
    screen.onPrev();
}

void onSelect()
{
    screen.onSelect();
}

void onUnselect()
{
    screen.onUnselect();
}