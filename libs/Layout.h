#ifndef LAYOUT_H
#define LAYOUT_H

#include "Dimensions.h"
#include "List.h"
#include "Container.h"

class Layout : public Container
{
protected:
    Layout * parent;
    List<Layout*> children;

public:
    Layout(Layout * parent);

    Position getPosition();
};

Layout::Layout(Layout * parent)
{
    this->parent = parent;
    if(parent != nullptr)
    {
        this->size = parent->size;
        this->position = parent->position;
    }
}

#endif