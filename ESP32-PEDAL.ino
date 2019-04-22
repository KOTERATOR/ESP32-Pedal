#include "Arduino.h"

#include "libs/GFX/List.h"
#include "libs/GFX/Dimensions.h"
#include "libs/GFX/View.h"
#include "libs/GFX/Layout.h"
#include "SSD1306.h"
#include "VirtualPotentiometer.h"
#include "PotentiometerView.h"
#include "Encoder.h"
#include "libs/IO/analogWrite.h"
#include "driver/adc.h"
#include "driver/dac.h"


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
        gfx.drawText((size.width - bounds.width)/2, (size.height - bounds.height)/2 + 1, name);
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
    pot.setValue(pot.getValue()+100);
}

void onCCW()
{
    pot.setValue(pot.getValue()-100);
}

Encoder enc(18, 19, onCW, onCCW);

void Task1code(void * parameter) 
{
    int prevP = 0, prevN = 0, counter = 0;
    for(;;) {
      
        int inp = adc1_get_raw(ADC1_CHANNEL_4);
        int inn = adc1_get_raw(ADC1_CHANNEL_5);
        innPot.setValue(inn);
        inpPot.setValue(inp);
        if(inp >= 4094 || inn >= 4094) analogWrite(14, HIGH);
        //dacWrite(25, inp*5);
        //Serial.print(" ");
        //dacWrite(26, inn*5);
        dac_output_voltage(DAC_CHANNEL_1, inp/16);
        dac_output_voltage(DAC_CHANNEL_2, inn/16);
        //vTaskDelay(10);
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
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    dac_output_voltage(DAC_CHANNEL_1, 0);
    dac_output_voltage(DAC_CHANNEL_2, 0);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_11);
    xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */
}



void loop()
{
    display.clear();
    //pot.setValue(val); //analogRead(15)
    analogWrite((uint8_t)14, pot.getValue(), 4095);
    mainLayout.draw();
    display.display();
    enc.run();
    //delay(10);
}