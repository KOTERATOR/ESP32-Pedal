#pragma once

#include "Layout.h"

class Activity : public Layout
{
public:
    
    Activity(LayoutMode layoutMode = LayoutMode::VERTICAL) : Layout(layoutMode, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64)) { isSelected = true; }

    virtual bool onExit();
};

bool Activity::onExit()
{
    
}