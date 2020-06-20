#pragma once

#include "Layout.h"

class Activity : public Layout
{
public:
    Activity * intentActivity = nullptr;
    Activity(LayoutMode layoutMode = LayoutMode::VERTICAL) : Layout(layoutMode, ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, Position(0, 0), Size(128, 64)) { isSelected = true; }

    void add(Container & container)
    {
        Layout::add(container);
        if(children.size() == 1)
        {
            selectedItemIndex = 0;
            children[selectedItemIndex]->onHover();
        }
    }

    virtual bool onExit();

    void onHover() {}
};

bool Activity::onExit()
{
    return true;
}

class StaticActivity : public Activity
{
public:
    StaticActivity(LayoutMode layoutMode = LayoutMode::VERTICAL) : Activity(layoutMode) {}

    void add(Container & container)
    {
        Layout::add(container);
    }
};