#include "Arduino.h"

#include "libs/List.h"
#include "libs/Dimensions.h"

List<int*> list;

void setup()
{
    Serial.begin(115200);
    int a = 25;
    list.add(&a);
    a = 35;
    list.add(&a);
    Serial.println(*list[0]);
    Position pos;
    pos.x = 20;
    pos.y = 30;
    Serial.println(pos.x);
}

void loop()
{

}