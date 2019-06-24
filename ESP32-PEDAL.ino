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


TaskHandle_t Task1;

//SSD1306 display = SSD1306(0x3c, 21, 22);

SSD1306 display(0x3c, 21, 22);
    const char * ssid = "Alex2G",
               * pass = "80081104";
PedalOTA * ota = nullptr;

void IRAM_ATTR Task1code(void *parameter)
{
    
    while (true)
    {
        ota->handle();
        pedal.controlsTick();
        pedal.draw();
        vTaskDelay(1);
    }
}

void setup()
{
    Serial.begin(115200);
    display.init();
    screen.setDisplay(&display); 
    StartupActivity startup = StartupActivity();
    pedal.setActivity(&startup);
    pedal.draw();
    delay(2000);

    ota = new PedalOTA(ssid, pass);
    pedal.startup();
    xTaskCreatePinnedToCore(Task1code, /* Function to implement the task */ "Task1",   /* Name of the task */ 10000,     /* Stack size in words */ NULL,      /* Task input parameter */ 0,         /* Priority of the task */ &Task1,    /* Task handle. */ 0);        /* Core where the task should run */
}


void loop()
{   
    //Serial.println(xPortGetCoreID());
    pedal.getInput();
    pedal.proceed();
}
