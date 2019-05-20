#pragma once

#include "Layout.h"

class Activity : public Layout
{
public:
    List<ControlView*> controls;
    Activity(LayoutMode layoutMode = LayoutMode::ABSOLUTE) : Layout(layoutMode, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64), nullptr) { isSelected = true; }

    virtual bool onExit() = 0;
};
