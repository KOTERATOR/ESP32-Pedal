#ifndef LAYOUT_H
#define LAYOUT_H

#include "Dimensions.h"
#include "../Utils/List.h"
#include "Container.h"
#include "ViewGFX.h"

enum class LayoutMode
{
    HORIZONTAL,
    VERTICAL,
    ABSOLUTE
};

class Layout : public Container
{
protected:
    Position defaultPosition;
    LayoutMode mode;
    size_t selectedItemIndex = 0;
    Container *selectedItem = nullptr;
    Container *prevSelectedItem = nullptr;

public:
    bool drawBorder = false;
    bool isSelected = false;
    List<Container *> children;
    Layout(LayoutMode layoutMode, ContainerSizeMode widthMode, ContainerSizeMode heightMode, Position position, Size size, ContainerMode containerMode, Layout *parent);
    void add(Container &container);
    void calculate();
    void draw(ViewGFX * gfx);
    void onDraw(ViewGFX * gfx);
    Size getChildrenBounds();

    virtual void onNext();
    virtual void onPrev();
    virtual void onSelect();
    virtual bool onUnselect();
    virtual void onHover();
    virtual void onUnhover();
    void centerSelectedItem();

    ~Layout()
    {
        children.~List();
    }

};

Layout::Layout(LayoutMode layoutMode, ContainerSizeMode widthMode, ContainerSizeMode heightMode, Position position = Position(0, 0), Size size = Size(0, 0), ContainerMode containerMode = ContainerMode::NORMAL, Layout *parent = nullptr) : Container(widthMode, heightMode, containerMode, position, size, parent), defaultPosition(position)
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
        if (childMode == ContainerMode::CENTER)
        {
            int x = (size.width - cW) / 2, y = (size.height - cH) / 2;
            Position childPos = c->getPosition();
            if (mode == LayoutMode::HORIZONTAL)
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

void Layout::draw(ViewGFX * gfx)
{
    gfx->setCurrentContainer(this);
    this->onDraw(gfx);
}

void Layout::onDraw(ViewGFX * gfx)
{
    for (int i = 0; i < children.size(); i++)
    {
        gfx->setCurrentContainer(children[i]);
        children[i]->draw(gfx);
        
    }
    
    if(drawBorder)
    {
        gfx->setCurrentContainer(this);
        for(int i = 0; i < size.width; i++)
        {
            for(int j = 0; j < size.height; j++)
            {
                if(j == 0 || j == size.height-1)
                {
                    if(i % 4 == 0)
                    {
                        gfx->drawPixel(i, j, Color::WHITE);
                        //Serial.println("WWWW");
                    }
                }
                else if(i == 0 || i == size.width-1)
                {
                    if(j % 4 == 0)
                    {
                        gfx->drawPixel(i, j, Color::WHITE);
                    }
                }
            }
        }
    }
}

void Layout::onSelect()
{
    if (selectedItem != nullptr)
    {
        selectedItem->onSelect();
    }
    else
    {
        if (!isSelected)
        {
            children[0]->onHover();
            centerSelectedItem();
            if(children.size() == 1)
            {
                selectedItem = children[0];
                selectedItem->onSelect();
            }
            isSelected = true;
        }
        else
        {
            selectedItem = children[selectedItemIndex];
            selectedItem->onSelect();
        }
    }
}

bool Layout::onUnselect()
{
    if (selectedItem != nullptr)
    {
        if (selectedItem->onUnselect())
        {
            //selectedItem->onUnhover();
            selectedItem = nullptr;
            if(children.size() == 1)
            {
                return this->onUnselect();
            }
        }
        return false;
    }
    else
    {
        children[selectedItemIndex]->onUnhover();
        selectedItemIndex = 0;
        centerSelectedItem();
        selectedItem = nullptr;
        isSelected = false;
        //setAbsolutePosition(defaultPosition);
        return true;
    }
}

void Layout::onNext()
{
    if (selectedItem != nullptr)
    {
        selectedItem->onNext();
    }
    else
    {
        if (selectedItemIndex < children.size() - 1)
        {
            children[selectedItemIndex]->onUnhover();
            selectedItemIndex++;
            children[selectedItemIndex]->onHover();
            centerSelectedItem();
        }
    }
}

void Layout::onPrev()
{
    if (selectedItem != nullptr)
    {
        selectedItem->onPrev();
    }
    else
    {
        if (selectedItemIndex > 0)
        {
            children[selectedItemIndex]->onUnhover();
            selectedItemIndex--;
            children[selectedItemIndex]->onHover();
            centerSelectedItem();
        }
    }
}

void Layout::onHover()
{
    if (selectedItem != nullptr)
    {
        selectedItem->onHover();
    }
    else
    {
        drawBorder = true;
    }
}

void Layout::onUnhover()
{
    if (selectedItem != nullptr)
    {
        selectedItem->onUnhover();
    }
    else
    {
        drawBorder = false;
    }
}

void Layout::centerSelectedItem()
{
    Container *child = children[selectedItemIndex];
    Container *absoluteParent = getAbsoluteParent();
    if (mode == LayoutMode::HORIZONTAL)
    {
        //setX(64 - children[selectedItemIndex]->getPosition().x - children[selectedItemIndex]->getSize().width / 2);
        if (child->getAbsolutePosition().x + child->getSize().width > 128)
        {
            int val = this->getPosition().x - (child->getAbsolutePosition().x + child->getSize().width - 128);
            this->setX(val);
        }
        else if (child->getAbsolutePosition().x < 0)
        {
            int val = this->getPosition().x - child->getAbsolutePosition().x;
            this->setX(val);
        }
    }
    else if (mode == LayoutMode::VERTICAL)
    {
        //setY(32 - children[selectedItemIndex]->getPosition().y - children[selectedItemIndex]->getSize().height / 2);
        if (child->getAbsolutePosition().y + child->getSize().height > 64)
        {
            int val = absoluteParent->getPosition().y - (child->getAbsolutePosition().y + child->getSize().height - 64);
            absoluteParent->setY(val);
        }
        else if (child->getAbsolutePosition().y < 0)
        {
            int val = absoluteParent->getPosition().y - child->getAbsolutePosition().y;
            absoluteParent->setY(val);
        }
    }
}

#endif