#include "Arduino.h"

#include "libs/List.h"
#include "libs/Dimensions.h"
#include "libs/View.h"
#include "libs/Layout.h"
#include "SSD1306.h"

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
    Border() : View(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::MATCH_PARENT, ContainerMode::ABSOLUTE, Position(0, 0), Size(0, 0), nullptr) {}
    void onDraw()
    {
        gfx.drawRect(0, 0, size.width, size.height, WHITE);
    }
};

class Circle : public View
{
  public:
    Circle() : View(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::MATCH_PARENT, ContainerMode::NORMAL, Position(0, 0), Size(16, 16)) {}

    void onDraw()
    {
        gfx.drawCircle(size.width / 2, size.height / 2, size.height / 2 - 1, WHITE);
    }
};

void setup()
{
    Serial.begin(115200);
    ViewGFX::setDisplay(&display);
    display.init();

    Layout m(LayoutMode::ABSOLUTE, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64));
    Layout c(LayoutMode::HORIZONTAL, ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::MATCH_PARENT, Position(0, 0), Size(31, 31));

    Border b;
    Circle cs;
    m.add(c);
    c.add(b);
    c.add(cs);
    printPos(c.getPosition());
    m.calculate();
    cs.draw();
    b.draw();

    //display.drawRect(0, 0, 128, 64);
    display.display();
    Serial.println("DISPLAYED");
    Serial.println((c.getWidthMode() == ContainerSizeMode::WRAP_CONTENT));
    printSize(c.getSize());
    printPos(c.getPosition());
}

void loop()
{

}