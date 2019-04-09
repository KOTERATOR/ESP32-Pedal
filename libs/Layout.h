#ifndef LAYOUT_H
#define LAYOUT_H

#include "Dimensions.h"
#include "List.h"
#include "Container.h"

enum LayoutSizeMode
{
    MATCH_PARENT,
    WRAP_CONTENT,
    ABSOLUTE,
    FIXED
};

enum LayoutMode
{
    HORIZONTAL,
    VERTICAL
};

class Layout : public Container
{
protected:
    LayoutSizeMode heightMode;
    LayoutSizeMode widthMode;
    LayoutMode mode;
    List<Container*> children;

public:
    Layout(Layout * parent);
    void add(Container & container);
    void calculate();
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

void Layout::calculate()
{
    int w = 0, h = 0;
    int maxW = 0, maxH = 0;
    for(int i = 0; i < children.size(); i++)
    {
        Container * c = children[i];
        if(mode == LayoutMode::HORIZONTAL)
        {
            w += c->size.width;
            h = c->size.height;
            if(maxH < h)
            {
                maxH = h;
            }
        }
        else if(mode == LayoutMode::VERTICAL)
        {
            h += c->size.height;
            w = c->size.width;
            if(maxW < w)
            {
                maxW = w;
            }
        }
    }
}

void Layout::add(Container & container)
{
    
}

#endif