#pragma once

#include "../GFX/Activity.h"
#include "../../ESP32Pedal/EffectsUnit.h"
#include "../Views/Header.h"
#include "../GFX/Layout.h"
#include "../Preset.h"
#include "../Views/EffectsUnitView.h"

class PresetActivity : public Activity
{
private:
    Preset * preset;
    Header presetHeader;
    Layout controlsLayout = Layout(LayoutMode::HORIZONTAL, ContainerSizeMode::WRAP_CONTENT, ContainerSizeMode::WRAP_CONTENT, Position(0, 0), Size(128, 48), ContainerMode::CENTER);
    List<EffectsUnitView*> unitsView = List<EffectsUnitView*>();
public:
    PresetActivity(Preset * preset) : Activity(), presetHeader(preset->name)
    {
        Serial.println("CTOR");
        this->preset = preset;
        add(presetHeader);
        for(int i = 0; i < preset->effects.size(); i++)
        {
            EffectsUnitView * ptr = new EffectsUnitView(preset->effects[i]);
            unitsView.add(ptr);
            controlsLayout.add(*((Container*)ptr));
        }
        add(controlsLayout);
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