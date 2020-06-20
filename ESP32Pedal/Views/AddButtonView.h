#pragma once

#include "../GFX/ClickView.h"

class AddButtonView : public ClickView
{
public:
    AddButtonView() : ClickView(ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, ContainerMode::NORMAL, Position(0, 0), Size(48, 48))
    {

    }

    void onDraw(ViewGFX * gfx)
    {
        Size plusSize(32, 32);
        int plusLineWidth = 4;
        gfx->fillCircle(size.width/2 , size.height/2 , size.width/2 - 4, Color::WHITE); 
        gfx->fillRect((size.width - plusLineWidth)/2, (size.height - plusSize.height)/2, plusLineWidth, plusSize.height, Color::BLACK);
        gfx->fillRect((size.width - plusSize.width)/2, (size.height - plusLineWidth)/2, plusSize.width, plusLineWidth, Color::BLACK);
    }
};