#pragma once

class Encoder
{
private:
    long prevMillis = 0;
    int pinA = 0, pinB = 0;
    bool A = false, B = false;
    void (*onCW)() = nullptr, (*onCCW)() = nullptr;
public:
    Encoder(int pinA, int pinB, void (*onCW)() = nullptr, void (*onCCW)() = nullptr);
    void run();
};

Encoder::Encoder(int pinA, int pinB, void (*onCW)(), void (*onCCW)())
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->onCW = onCW;
    this->onCCW = onCCW;
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
}

void Encoder::run()
{
    long now = millis();
    if(now - prevMillis >= 1)
    {
        bool newA = digitalRead(pinA), newB = digitalRead(pinB);
        if(B && !newB && !newA)
        {
            onCW();
        }
        else if(A && !newA && !B)
        {
            onCCW();
        }
        
        A = newA;
        B = newB;
        prevMillis = now;
    }
}