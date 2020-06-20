#include "Arduino.h"

#include "ESP32Pedal/Utils/List.h"
#include "ESP32Pedal/Views/Header.h"
#include "ESP32Pedal/GFX/Layout.h"
#include "ESP32Pedal/Controls/VirtualPotentiometer.h"
#include "ESP32Pedal/IO/analogWrite.h"
#include "ESP32Pedal/Pedal.h"
#include "ESP32Pedal/Utils/PedalOTA.h"
#include "esp_int_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ESP32Pedal/Activities/StartupActivity.h"
#include "ESP32Pedal/Activities/PresetActivity.h"


TaskHandle_t Task1;

//SSD1306 display = SSD1306(0x3c, 21, 22);

SSD1306 display(0x3c, 21, 22);
    const char * ssid = "Alex2G",
               * pass = "80081104";
PedalOTA * ota = nullptr;

void Task1code(void *parameter)
{
    start();
    pedal.startup();
    for (;;)
    {
        ota->handle();
        pedal.draw();
        delayMicroseconds(100); 
    }
}

void start()
{
    pedal.onStartup += []()
    {
        pedal.setActivity(new PresetActivity(pedal.preset));
    };

    display.init();
    screen.setDisplay(&display);
    pedal.draw();

    pedal.setActivity(new StartupActivity());
    pedal.draw();
    int breathDelay = 8;
    for(int i = 0; i < 256; i++)
    {
        pedal.setLED(i);
        delay(breathDelay);
    }
    for(int i = 255; i >= 0; i--)
    {
        pedal.setLED(i);
        delay(breathDelay);
    }

    ota = new PedalOTA(ssid, pass);
}

void setup()
{
    Serial.begin(115200);
    Serial.println(ESP.getFreeHeap());
    
    

    xTaskCreatePinnedToCore(Task1code, /* Function to implement the task */ "Task1",   /* Name of the task */ 20000,     /* Stack size in words */ NULL,      /* Task input parameter */ 1,         /* Priority of the task */ &Task1,    /* Task handle. */ 0);        /* Core where the task should run */
    //delay(2000);
    //screen.deallocActivity();

    //Container * lst[10] = {};
    /*List<Container*> lst;
    Serial.println(ESP.getFreeHeap());
    for(int i = 0; i < 10; i++)
    {
        auto ptr = (new Container(ContainerSizeMode::FIXED, ContainerSizeMode::FIXED));
        //lst[i] = ptr;
        lst.add(ptr);
    }
    Serial.println(ESP.getFreeHeap());
    for(int i = 0; i < 10; i++)
    {
        delete lst[i];
    }
    lst.~List();
    Serial.println(ESP.getFreeHeap());
    delay(4000);*/
}

void loop()
{   
    pedal.controlsTick();
    pedal.getInput();
    pedal.proceed();
}
