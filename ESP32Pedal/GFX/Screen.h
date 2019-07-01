#pragma once

#include "Activity.h"
#include "ViewGFX.h"
#include "SSD1306.h"

class Screen
{
private:
    Activity * currentActivity = nullptr;
    bool isActivityDisposed = false;
    SSD1306 * display_ptr = nullptr;
public:

    ViewGFX gfx;

    Screen() : gfx(Size(128, 64))
    {   

    }

    void setDisplay(SSD1306 * display)
    {
        this->display_ptr = display;
        ViewGFX::setDisplay(display_ptr);
    }

    void setActivity(Activity * activity)
    {
        deallocActivity();
        currentActivity = activity;
        if(activity != nullptr)
        {
            currentActivity->calculate();
        }
    }

    void intent(Activity * activity)
    {
        if(activity != nullptr)
        {
            activity->intentActivity = currentActivity;
            currentActivity = activity;
            currentActivity->calculate();
        }
    }

    void deallocActivity()
    {
        isActivityDisposed = true;
        if(currentActivity != nullptr)
        {
            delete currentActivity;
            Serial.println(ESP.getFreeHeap());
            currentActivity = nullptr;
        }
        isActivityDisposed = false;
    }

    void draw()
    {
        display_ptr->clear();
        if(currentActivity != nullptr && !isActivityDisposed)
        {
            gfx.clear();
            currentActivity->draw(&gfx);
            gfx.draw();
        }
        display_ptr->display();
    }

    void onNext()
    {
        if(currentActivity != nullptr && !isActivityDisposed)
        {
            currentActivity->onNext();
        }
    }

    void onPrev()
    {
        if(currentActivity != nullptr && !isActivityDisposed)
        {
            currentActivity->onPrev();
        }
    }

    void onSelect()
    {
        if(currentActivity != nullptr && !isActivityDisposed)
        {
            currentActivity->onSelect();
        }
    }

    void onUnselect()
    {
        if(currentActivity != nullptr && !isActivityDisposed)
        {
            if(currentActivity->onUnselect())
            {
                currentActivity->onExit();
                if(currentActivity->intentActivity != nullptr)
                {
                    currentActivity->intentActivity->onUnselect();
                    setActivity(currentActivity->intentActivity);
                }
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