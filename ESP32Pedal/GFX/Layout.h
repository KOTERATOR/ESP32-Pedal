#ifndef LAYOUT_H
#define LAYOUT_H

#include "Dimensions.h"
#include "../Utils/List.h"
#include "Container.h"
#include "View.h"

enum class LayoutMode
{
    HORIZONTAL,
    VERTICAL,
    ABSOLUTE
};

class Layout : public Container
{
  protected:
    LayoutMode mode;
    

  public:
    List<Container *> children;
    Layout(LayoutMode layoutMode, ContainerSizeMode widthMode, ContainerSizeMode heightMode, Position position, Size size, ContainerMode containerMode, Layout *parent);
    void add(Container &container);
    void calculate();
    void draw();
    Size getChildrenBounds();
};

Layout::Layout(LayoutMode layoutMode, ContainerSizeMode widthMode, ContainerSizeMode heightMode, Position position = Position(0, 0), Size size = Size(0, 0), ContainerMode containerMode = ContainerMode::NORMAL, Layout *parent = nullptr) : Container(widthMode, heightMode, containerMode, position, size, parent)
{
    this->parent = parent;
    if (parent != nullptr)
    {
        parent->add((*this));
    }
    this->mode = layoutMode;
}

void Layout::calculate()
{

    Size childrenSize = getChildrenBounds();

    if (widthMode == ContainerSizeMode::WRAP_CONTENT)
    {
        size.width = childrenSize.width;
    }
    else if (widthMode == ContainerSizeMode::MATCH_PARENT)
    {
        if (parent != nullptr)
            size.width = parent->getSize().width;
    }

    if (heightMode == ContainerSizeMode::WRAP_CONTENT)
    {
        size.height = childrenSize.height;
    }
    else if (heightMode == ContainerSizeMode::MATCH_PARENT)
    {
        if (parent != nullptr)
            size.height = parent->getSize().height;
    }

    int w = 0, h = 0;
    int maxW = 0, maxH = 0;
    for (int i = 0; i < children.size(); i++)
    {
        Container *c = children[i];

        c->calculate();

        Size s = c->getSize();
        int cH = s.height, cW = s.width;
        if (c->getWidthMode() == ContainerSizeMode::MATCH_PARENT)
        {
            cW = size.width;
        }
        if (c->getHeightMode() == ContainerSizeMode::MATCH_PARENT)
        {
            cH = size.height;
        }
        c->setSize(Size(cW, cH));

        ContainerMode childMode = c->getContainerMode();
        if (childMode != ContainerMode::ABSOLUTE)
        {
            if (mode == LayoutMode::HORIZONTAL)
            {
                c->setX(w);
                w += s.width;
                h = s.height;
            }
            else if (mode == LayoutMode::VERTICAL)
            {
                c->setY(h);
                h += s.height;
                w = s.width;
            }
            if (maxH < h)
            {
                maxH = h;
            }
            if (maxW < w)
            {
                maxW = w;
            }
        }
        if(childMode == ContainerMode::CENTER)
        {
            int x = (size.width - cW)/2, y = (size.height - cH)/2;
            Position childPos = c->getPosition();
            if(mode == LayoutMode::HORIZONTAL)
            {
                x = childPos.x;
            }
            else if (mode == LayoutMode::VERTICAL)
            {
                y = childPos.y;
            }
            c->setPosition(Position(x, y));
        }
    }
}

void Layout::add(Container &container)
{
    container.setParent(this);
    children.add(&container);
}

Size Layout::getChildrenBounds()
{
    int w = 0, h = 0;
    int maxW = 0, maxH = 0;
    for (int i = 0; i < children.size(); i++)
    {
        Container *c = children[i];

        c->calculate();
        if (c->getContainerMode() == ContainerMode::ABSOLUTE)
        {
            continue;
        }
        Size s = c->getSize();

        if (mode == LayoutMode::HORIZONTAL)
        {
            w += s.width;
            h = s.height;
        }
        else if (mode == LayoutMode::VERTICAL)
        {
            h += s.height;
            w = s.width;
        }
        if (maxH < h)
        {
            maxH = h;
        }
        if (maxW < w)
        {
            maxW = w;
        }
    }
    return Size(maxW, maxH);
}

void Layout::draw()
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->draw();
    }
}

#endif