#pragma once

#include "Dimensions.h"

class ViewGFX;

enum class ContainerSizeMode
{
    MATCH_PARENT,
    WRAP_CONTENT,
    FIXED
};

enum class ContainerMode
{
    NORMAL,
    ABSOLUTE,
    CENTER
};

class Container
{
  protected:
    Container *parent;

    ContainerSizeMode heightMode;
    ContainerSizeMode widthMode;

    ContainerMode containerMode;

    Position position;
    Size size;

    

  public:
    Container(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode, Position position, Size size, Container *parent);

    Position getPosition();
    Position getAbsolutePosition();
    void setPosition(Position position);
    void setX(int x) { position.x = x; }
    void setY(int y) { position.y = y; }
    void setAbsolutePosition(Position position);

    ContainerMode getContainerMode() { return containerMode; }

    Size getSize();
    Size & getReferenceSize();
    virtual void setSize(Size size);
    void setWidth(int width) { setSize(Size(width, size.height)); }
    void setHeight(int height) { setSize(Size(size.width, height)); }

    Container *getParent();
    Container * getAbsoluteParent();
    void setParent(Container *parent);
    virtual void calculate() {};
    virtual void draw(ViewGFX * gfx);

    ContainerSizeMode getWidthMode() { return widthMode; }
    ContainerSizeMode getHeightMode() { return heightMode; }

    virtual void onNext() {}
    virtual void onPrev() {}
    virtual void onSelect() {}
    virtual bool onUnselect() {}
    virtual void onHover() {}
    virtual void onUnhover() {}

    virtual void onDraw(ViewGFX * gfx) {}

    virtual ~Container()
    {
    }
};

Container::Container(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode = ContainerMode::NORMAL, Position position = Position(0, 0), Size size = Size(0, 0), Container *parent = nullptr)
{
    this->parent = parent;
    this->widthMode = widthMode;
    this->heightMode = heightMode;
    this->position = position;
    this->size = size;
    this->containerMode = containerMode;
}

void Container::draw(ViewGFX * gfx)
{
    onDraw(gfx);
    //gfx.draw();
}

Position Container::getPosition()
{
    return position;
}

Position Container::getAbsolutePosition()
{
    Position pos(0, 0);
    if (parent != nullptr)
    {
        pos = parent->getAbsolutePosition();
    }
    pos = pos + this->position;
    return pos;
}

void Container::setPosition(Position position)
{
    this->position = position;
}

void Container::setAbsolutePosition(Position position)
{
    Position absolute = getAbsolutePosition();
    this->position = position - absolute;
}

Size Container::getSize()
{
    return size;
}

Size & Container::getReferenceSize()
{
    return size;
}

void Container::setSize(Size size)
{
    this->size = size;
}

Container *Container::getParent()
{
    return parent;
}

Container * Container::getAbsoluteParent()
{
    if(parent != nullptr)
    {
        return parent->getAbsoluteParent();
    }
    else
    {
        return this;
    }
}

void Container::setParent(Container *parent)
{
    this->parent = parent;
}
