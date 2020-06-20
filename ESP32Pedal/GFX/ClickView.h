#pragma once

#include "EventView.h"

class ClickView : public EventView
{
public:

    ClickView(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode = ContainerMode::NORMAL, Position position = Position(0, 0), Size size = Size(0, 0), Container *parent = nullptr) : EventView(widthMode, heightMode, containerMode, position, size, parent)
    {
        
    }

    void onSelect();
    bool onUnselect();
    void onNext();
    void onPrev();
};

void ClickView::onSelect()
{
    View::onSelect();
    if(onSelectEventFunc) onSelectEventFunc();
    if(parent != nullptr)
    {
        parent->onUnselect();
    }
}

bool ClickView::onUnselect()
{
    bool val = View::onUnselect();
    if(onUnselectEventFunc) onUnselectEventFunc();
    return val;
}

void ClickView::onNext()
{
    if(onNextEventFunc) onNextEventFunc();
    View::onNext();
}

void ClickView::onPrev()
{
    if(onPrevEventFunc) onPrevEventFunc();
    View::onPrev();
}

