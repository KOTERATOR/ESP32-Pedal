#pragma once

#include "../GFX/Activity.h"
#include "../../ESP32Pedal/EffectsUnit.h"
#include "../Views/Header.h"
#include "../GFX/Layout.h"
#include "../Preset.h"
#include "../Views/EffectsUnit/EffectsUnitView.h"
#include "../Views/ArrowView.h"
#include "../Views/AddButtonView.h"
#include "../Activities/AddEffectActivity.h"
#include "../Views/TextBox.h"

class PresetActivity : public Activity
{
private:
    Preset * preset;
    TextBox presetHeader;
    AddButtonView addEffectButton = AddButtonView();
    Layout controlsLayout = Layout(LayoutMode::HORIZONTAL, ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0), Size(128, 48), ContainerMode::NORMAL);
    List<EffectsUnitView*> unitsView = List<EffectsUnitView*>();
    List<ArrowView> arrows = List<ArrowView>();
public:
    PresetActivity(Preset * preset) : Activity(), presetHeader(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT, preset->name)
    {
        this->preset = preset;
        add(presetHeader);
        for(int i = 0; i < preset->effects.size(); i++)
        {
            EffectsUnitView * ptr = new EffectsUnitView(preset->effects[i]);
            ArrowView arrow;
            unitsView.add(ptr);
            arrows.add(arrow);
            controlsLayout.add(*((Container*)ptr));
            controlsLayout.add(arrows[i]);
        }
        addEffectButton.onSelectEvent([=](){ pedal.intent(new AddEffectActivity(preset));});
        controlsLayout.add(addEffectButton);
        add(controlsLayout);
    }

    void onDraw(ViewGFX * gfx)
    {
        if(preset->effects.size() != unitsView.size())
        {
            controlsLayout.clear();
            for(int i = 0; i < unitsView.size(); i++)
            {
                EffectsUnitView * view = unitsView[i];
                unitsView.removeAt(i);
                delete view;
            }
            unitsView.clear();
            arrows.clear();
            for(int i = 0; i < preset->effects.size(); i++)
            {
                EffectsUnitView * ptr = new EffectsUnitView(preset->effects[i]);
                ArrowView arrow;
                unitsView.add(ptr);
                arrows.add(arrow);
                controlsLayout.add(*((Container*)ptr));
                controlsLayout.add(arrows[i]);
            }
            controlsLayout.add(addEffectButton);
            calculate();
        }
        Activity::onDraw(gfx);
    }

    ~PresetActivity()
    {
        for(int i = 0; i < unitsView.size(); i++)
        {
            EffectsUnitView * view = unitsView[i];
            //unitsView.removeAt(i);
            delete view;
        }
    }
};