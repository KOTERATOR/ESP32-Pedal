#pragma once

#include "Container.h"
#include "ViewGFX.h"

class Layout;

class View : public Container
{
  protected:

  public:
    bool drawBorder = false, isSelected = false;
    View(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode, Position position, Size size, Container *parent);
    void draw(ViewGFX * gfx);
    void calculate() {}
    virtual void onDraw(ViewGFX * gfx) {}
    virtual void onHover();
    virtual void onUnhover();
    virtual void onSelect();
    virtual bool onUnselect();
    virtual void onNext();
    virtual void onPrev();

    bool isView() { return true; }
};

View::View(ContainerSizeMode widthMode, ContainerSizeMode heightMode, ContainerMode containerMode = ContainerMode::NORMAL, Position position = Position(0, 0), Size size = Size(0, 0), Container *parent = nullptr) : Container(widthMode, heightMode, containerMode, position, size, parent)
{
    
}

void View::draw(ViewGFX * gfx)
{
    if(isSelected)
    {
        gfx->drawRect(0, 0, size.width, size.height, Color::WHITE);
    }
    else if(drawBorder)
    {
        for(int i = 0; i < size.width; i++)
        {
            for(int j = 0; j < size.height; j++)
            {
                if(j == 0 || j == size.height-1)
                {
                    if(i % 2 == 0)
                    {
                        gfx->drawPixel(i, j, Color::WHITE);
                    }
                }
                else if(i == 0 || i == size.width-1)
                {
                    if(j % 2 == 0)
                    {
                        gfx->drawPixel(i, j, Color::WHITE);
                    }
                }
            }
        }
    }
    
    onDraw(gfx);
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
    isSelected = true;
}

bool View::onUnselect()
{
    isSelected = false;
    return true;
}

void View::onNext()
{
}

void View::onPrev()
{
}