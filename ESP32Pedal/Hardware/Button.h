#pragma once

#include "../Utils/Event.h"

class Button
{
    protected:
    bool _isDown = false;
    bool _isHolded = false;
    int _debounceInterval = 20;
    int _doubleClickInterval = 400;
    int _holdInterval = 500;
    int _lastClickMillis = 0;
    int _lastPressMillis = 0;
    int _lastCheckMillis = 0;
    public:

    int pin;
    bool enabled = true;
    bool isDown();
    void tick();
    void setDebounce(int millis);
    void setDoubleClickInterval(int millis);
    Event<> onPress, onRelease, onClick, onDoubleClick, onOnceClick, onHold;

    Button();
    Button(int pin, int pinmode = INPUT_PULLUP);
};

Button::Button()
{

}

Button::Button(int pin, int pinmode)
{
    this->pin = pin;
    pinMode(pin, pinmode);
}

bool Button::isDown()
{ 
    return _isDown;
}

void Button::setDebounce(int millis)
{
    this->_debounceInterval = millis;
}

void Button::setDoubleClickInterval(int millis)
{
    this->_doubleClickInterval = millis;
}

void Button::tick()
{
    if(enabled && millis()-_lastCheckMillis >= _debounceInterval)
    {
        _lastCheckMillis = millis();
        if(!digitalRead(pin))
        {
            if(!_isDown)
            {
                _isDown = true;
                _lastPressMillis = _lastCheckMillis;
                onPress();
            }
            else
            {
                if(_lastCheckMillis - _lastPressMillis >= _holdInterval)
                {
                    if(!_isHolded)
                    {
                        _isHolded = true;
                        onHold();
                    }
                }
                else
                {
                    _isHolded = false;
                }
            }
        }
        else
        {
            if(_isDown)
            {
                _isDown = false;
                onRelease();

                if(_lastCheckMillis - _lastClickMillis <= _doubleClickInterval)
                {
                    onDoubleClick();
                }
                
                else
                {
                    if(!_isHolded)
                        onOnceClick();
                }
                onClick();
                _lastClickMillis = _lastCheckMillis;
            }
        }
    }
}