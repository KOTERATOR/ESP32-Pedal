#pragma once

#include "Activity.h"

class Screen
{
private:
    Activity * currentActivity;
public:
    void setActivity(Activity * activity);
};