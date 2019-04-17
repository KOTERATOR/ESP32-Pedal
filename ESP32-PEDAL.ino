#include "Arduino.h"

#include "libs/List.h"
#include "libs/Dimensions.h"
#include "libs/View.h"
#include "libs/Layout.h"
#include "SSD1306.h"
#include "VirtualPotentiometer.h"
#include "PotentiometerView.h"

SSD1306 display(0x3c, 21, 22);

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

class Border : public View
{
  public:
    Border() : View(ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, ContainerMode::ABSOLUTE, Position(0, 0), Size(0, 0), nullptr) {}
    void onDraw()
    {
        gfx.drawRect(0, 0, size.width, size.height, WHITE);
    }
};

class Circle : public View
{
  public:
    Circle() : View(ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, ContainerMode::NORMAL, Position(0, 0), Size(16, 16)) {}

    void onDraw()
    {
        gfx.drawCircle(size.width / 2, size.height / 2, size.height / 2 - 1, WHITE);
    }
};

Layout m(LayoutMode::ABSOLUTE, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64));
Layout c(LayoutMode::HORIZONTAL, ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::MATCH_PARENT, Position(0, 0), Size(31, 31));

VirtualPotentiometer pot;
PotentiometerView potView(pot, "TONE");
PotentiometerView potView2(pot, "VOL");
Circle circle;

void setup()
{
    Serial.begin(115200);
    ViewGFX::setDisplay(&display);
    display.init();
    
    c.add(potView);
    c.add(potView2);
    potView2.drawBorder = true;
    m.add(c);
    m.calculate();
}

void loop()
{
    for(int i = 0; i < 4096; i+=10)
    {
        display.clear();
        pot.setValue(i);
        m.draw();
        display.display();
        delay(10);
    }
}