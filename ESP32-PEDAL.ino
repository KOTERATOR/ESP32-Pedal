#include "Arduino.h"

#include "ESP32Pedal/Utils/List.h"
#include "ESP32Pedal/GFX/Dimensions.h"
#include "ESP32Pedal/GFX/View.h"
#include "ESP32Pedal/Views/Header.h"
#include "ESP32Pedal/GFX/Layout.h"
#include "SSD1306.h"
#include "ESP32Pedal/Controls/VirtualPotentiometer.h"
#include "PotentiometerView.h"
#include "ESP32Pedal/Hardware/Encoder.h"
#include "ESP32Pedal/IO/analogWrite.h"
#include "ESP32Pedal/Pedal.h"
#include "ESP32Pedal/Hardware/HardwareTimer.h"
#include "ESP32Pedal/Hardware/Button.h"

SSD1306 display(0x3c, 21, 22);
TaskHandle_t Task1;

Layout mainLayout(LayoutMode::VERTICAL, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64));
Layout header(LayoutMode::HORIZONTAL, ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0));
Layout c(LayoutMode::HORIZONTAL, ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0), Size(128, 48));

VirtualPotentiometer pot;
VirtualPotentiometer innPot, inpPot;
PotentiometerView potView(pot, "VOLUME");
PotentiometerView innView(innPot, "INN");
PotentiometerView inpView(inpPot, "INP");
PotentiometerView ippView(inpPot, "IPP");
Header effectHeader("OCTAVER");

Pedal pedal;

int val = 0;

Encoder enc(18, 19, 4);

volatile int samples = 0;

void IRAM_ATTR Task1code(void *parameter)
{
    int16_t in = 0;
    for (;;)
    {
            display.clear();
        //pot.setValue(val); //analogRead(15)
        //analogWrite((uint8_t)14, pot.getValue(), 4095);
        mainLayout.draw();
        display.display();
        enc.tick();
        pedal.controlsTick();
        vTaskDelay(1);
    }
}


void setup()
{
    Serial.begin(115200);
    pinMode(14, OUTPUT);
    ViewGFX::setDisplay(&display);
    display.init();
    mainLayout.add(header);
    header.add(effectHeader);
    c.add(innView);
    c.add(inpView);
    c.add(potView);
    c.add(ippView);
    //effectHeader.drawBorder = true;
    mainLayout.add(c);
    mainLayout.calculate();
    mainLayout.isSelected = true;
    header.drawBorder = true;
    digitalWrite(14, true);
    analogWriteResolution(14, 12);
    //c.drawBorder = true;
    xTaskCreatePinnedToCore(
        Task1code, /* Function to implement the task */
        "Task1",   /* Name of the task */
        10000,     /* Stack size in words */
        NULL,      /* Task input parameter */
        0,         /* Priority of the task */
        &Task1,    /* Task handle. */
        0);        /* Core where the task should run */

        enc.onCW += []() { mainLayout.onNext(); };
        enc.onCCW += []() { mainLayout.onPrev(); };
        enc.button.onOnceClick += []() { mainLayout.onSelect(); };
        enc.button.onDoubleClick += []() { Serial.println("DOUBLE CLICK"); };
        enc.button.onHold += []() { mainLayout.onUnselect(); };
}

int prevMils = 0;

void loop()
{
    pedal.getInput();
    pedal.proceed();
}