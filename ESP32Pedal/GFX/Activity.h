#pragma once

#include "Layout.h"
#include "../Views/ControlView.h"

class Activity : public Layout
{
public:
    List<ControlView*> controls;
    Activity(LayoutMode layoutMode = LayoutMode::ABSOLUTE) : Layout(layoutMode, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64), nullptr) {}

    void onNext();
    void onPrev();
};

void Activity::onNext()
{
    
}