#pragma once

#include "../GFX/View.h"

class ArrowView : public View
{
  protected:
    Size arrowSize = Size(16, 16), arrowRectSize = Size(8, 8);
  public:
    ArrowView(Size size = Size(16, 48)) : View(ContainerSizeMode::FIXED, ContainerSizeMode::FIXED, ContainerMode::NORMAL, Position(0, 0), size) {
      isSelectable = false;
    }

    void onDraw(ViewGFX * gfx)
    {
        int hCenter = size.height/2;
        int rectTopPointY = hCenter - arrowRectSize.height/2;
        gfx->fillRect(0, rectTopPointY, arrowRectSize.width, arrowRectSize.height, Color::WHITE);
        gfx->fillTriangle(arrowRectSize.width, (size.height - arrowSize.height)/2 + 1, arrowRectSize.width, (size.height + arrowSize.height)/2 - 1, size.width-1, hCenter-1, Color::WHITE);
    }
};