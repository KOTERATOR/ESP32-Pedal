#include "Arduino.h"

#include "libs/List.h"

List<int*> list;

void setup()
{
    Serial.begin(115200);
    int a = 25;
    list.add(&a);
    a = 35;
    list.add(&a);
    Serial.println(*list[0]);
}

void loop()
{

}