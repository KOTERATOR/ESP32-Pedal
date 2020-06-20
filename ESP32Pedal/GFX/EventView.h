#pragma once

#include "View.h"
#include <functional>

typedef std::function<void(void)> EventViewEvent;

class EventView : public View
{
protected:
    EventViewEvent onNextEventFunc, onPrevEventFunc, onSelectEventFunc, onUnselectEventFunc;
public:

    EventView(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode = ContainerMode::NORMAL, Position position = Position(0, 0), Size size = Size(0, 0), Container *parent = nullptr) : View(widthMode, heightMode, containerMode, position, size, parent)
    {
        
    }

    EventView & onNextEvent(EventViewEvent event)
    {
        onNextEventFunc = event; 
        return (*this);
    }

    EventView & onPrevEvent(EventViewEvent event)
    {
        onPrevEventFunc = event; 
        return (*this);
    }

    EventView & onSelectEvent(EventViewEvent event)
    {
        onSelectEventFunc = event;
        return (*this);
    }

    EventView & onUnselectEvent(EventViewEvent event)
    {
        onNextEventFunc = event;
        return (*this);
    }

    virtual void onSelect();
    virtual bool onUnselect();
    virtual void onNext();
    virtual void onPrev();
};

void EventView::onSelect()
{
    View::onSelect();
    if(onSelectEventFunc) onSelectEventFunc();
}

bool EventView::onUnselect()
{
    bool val = View::onUnselect();
    if(onUnselectEventFunc) onUnselectEventFunc();
    return val;
}

void EventView::onNext()
{
    if(onNextEventFunc) onNextEventFunc();
    View::onNext();
}

void EventView::onPrev()
{
    if(onPrevEventFunc) onPrevEventFunc();
    View::onPrev();
}

