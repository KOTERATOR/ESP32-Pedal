#pragma once

#include "Container.h"
#include "ViewGFX.h"

class View : public Container
{
  protected:
    ViewGFX gfx;
    bool isSelectable = true;
  public:
    
    bool drawBorder = false;
    View(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode, Position position, Size size, Container *parent);
    void setSize(Size size);
    void draw();
    void calculate() {}
    virtual void onDraw() = 0;
    virtual void onHover();
    virtual void onUnhover();
    virtual void onSelect();
    virtual void onUnselect();
    virtual void onNext();
    virtual void onPrev();
};

View::View(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode = ContainerMode::NORMAL, Position position = Position(0, 0), Size size = Size(0, 0), Container *parent = nullptr) : Container(widthMode, heightMode, containerMode, position, size, parent), gfx(size)
{

}

void View::setSize(Size size)
{
    Container::setSize(size);
    gfx.setSize(size);
}

void View::draw()
{
    gfx.setOffset(this->getAbsolutePosition());
    gfx.clear();
    if(drawBorder)
    {
      gfx.drawRect(0, 0, size.width, size.height, Color::WHITE);
    }
    onDraw();
    gfx.draw();
}

void View::onHover()
{
    drawBorder = true;
}

void View::onUnhover()
{
    drawBorder = false;
}

void View::onSelect()
{
    drawBorder = true;
}

void View::onUnselect()
{
    drawBorder = false;
}

void View::onNext()
{

}

void View::onPrev()
{

}