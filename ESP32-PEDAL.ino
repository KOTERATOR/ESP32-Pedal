#include "Arduino.h"

#include "ESP32Pedal/Utils/List.h"
#include "ESP32Pedal/Views/Header.h"
#include "ESP32Pedal/GFX/Layout.h"
#include "ESP32Pedal/Controls/VirtualPotentiometer.h"
#include "ESP32Pedal/IO/analogWrite.h"
#include "ESP32Pedal/Pedal.h"


TaskHandle_t Task1;

Pedal pedal;
SSD1306 display(0x3c, 21, 22);


void IRAM_ATTR Task1code(void *parameter)
{
    for (;;)
    {
        pedal.controlsTick();
        pedal.draw();
        vTaskDelay(1);
    }
}


void setup()
{
    Serial.begin(115200);
    pinMode(14, OUTPUT);
    display.init();
    screen.setDisplay(&display);
    digitalWrite(14, true);
    analogWriteResolution(14, 12);
    //c.drawBorder = true;
    xTaskCreatePinnedToCore(Task1code, /* Function to implement the task */ "Task1",   /* Name of the task */ 10000,     /* Stack size in words */ NULL,      /* Task input parameter */ 0,         /* Priority of the task */ &Task1,    /* Task handle. */ 0);        /* Core where the task should run */
        
}


void loop()
{
    pedal.getInput();
    pedal.proceed();
}