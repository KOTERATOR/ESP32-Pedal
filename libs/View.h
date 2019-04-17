#pragma once

#include "Container.h"
#include "Layout.h"
#include "ViewGFX.h"

class View : public Container
{
  protected:
    ViewGFX gfx;

  public:
    bool drawBorder = false;
    View(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode, Position position, Size size, Layout *parent);
    void setSize(Size size);
    void draw();
    void calculate() {}
    virtual void onDraw() = 0;
};

View::View(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode = ContainerMode::NORMAL, Position position = Position(0, 0), Size size = Size(0, 0), Layout *parent = nullptr) : Container(widthMode, heightMode, containerMode, position, size, parent), gfx(size)
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