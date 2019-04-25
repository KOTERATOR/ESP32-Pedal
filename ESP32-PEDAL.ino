#include "Arduino.h"

#include "ESP32Pedal/Utils/List.h"
#include "ESP32Pedal/GFX/Dimensions.h"
#include "ESP32Pedal/GFX/View.h"
#include "ESP32Pedal/GFX/Layout.h"
#include "SSD1306.h"
#include "VirtualPotentiometer.h"
#include "PotentiometerView.h"
#include "Encoder.h"
#include "ESP32Pedal/IO/analogWrite.h"
#include "ESP32Pedal/Pedal.h"
#include "ESP32Pedal/Hardware/HardwareTimer.h"

SSD1306 display(0x3c, 21, 22);
TaskHandle_t Task1;

void printPos(Position pos)
{
    Serial.print(pos.x);
    Serial.print(' ');
    Serial.println(pos.y);
}

void printSize(Size s)
{
    Serial.print(s.width);
    Serial.print(' ');
    Serial.println(s.height);
}

class Header : public View
{
  private:
    String name;

  public:
    Header(String name) : View(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::FIXED, ContainerMode::NORMAL, Position(0, 0), Size(128, 16)), name(name) {}

    void onDraw()
    {
        Size bounds = gfx.getTextBounds(name);
        gfx.drawText((size.width - bounds.width) / 2, (size.height - bounds.height) / 2 + 1, name);
        //gfx.drawText(0, 8, name, Color::WHITE);
    }
};

Layout mainLayout(LayoutMode::VERTICAL, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64));
Layout header(LayoutMode::HORIZONTAL, ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0));
Layout c(LayoutMode::HORIZONTAL, ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0), Size(31, 31), ContainerMode::CENTER);

VirtualPotentiometer pot;
VirtualPotentiometer innPot, inpPot;
PotentiometerView potView(pot, "VOLUME");
PotentiometerView innView(innPot, "INN");
PotentiometerView inpView(inpPot, "INP");
Header effectHeader("OCTAVER");

int val = 0;

void onCW()
{
    pot.setValue(pot.getValue() + 100);
}

void onCCW()
{
    pot.setValue(pot.getValue() - 100);
}

Encoder enc(18, 19, onCW, onCCW);

void Task1code(void *parameter)
{
    int prevP = 0, prevN = 0, counter = 0;

    for (;;)
    {
        display.clear();
        //pot.setValue(val); //analogRead(15)
        analogWrite((uint8_t)14, pot.getValue(), 4095);
        mainLayout.draw();
        display.display();
        enc.run();
        vTaskDelay(1);
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(19, INPUT_PULLUP);
    pinMode(18, INPUT_PULLUP);
    pinMode(14, OUTPUT);
    ViewGFX::setDisplay(&display);
    display.init();
    mainLayout.add(header);
    header.add(effectHeader);
    c.add(innView);
    c.add(inpView);
    c.add(potView);
    effectHeader.drawBorder = true;
    mainLayout.add(c);
    mainLayout.calculate();
    digitalWrite(14, true);
    analogWriteResolution(14, 12);

    xTaskCreatePinnedToCore(
        Task1code, /* Function to implement the task */
        "Task1",   /* Name of the task */
        10000,     /* Stack size in words */
        NULL,      /* Task input parameter */
        0,         /* Priority of the task */
        &Task1,    /* Task handle. */
        0);        /* Core where the task should run */
    pedal = new Pedal(12000);
    
}

Audio audio;
int counter = 0, prevMillis = 0;
int16_t in1, in2;
void loop()
{
    pedal->getInput();
    pedal->proceed();
    counter++;
    if(millis() - prevMillis >= 1000)
    {
        Serial.println(counter);
        counter = 0;
        prevMillis = millis();
    }
}