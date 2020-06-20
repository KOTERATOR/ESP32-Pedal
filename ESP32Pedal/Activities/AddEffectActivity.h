#pragma once

#include "../GFX/Activity.h"
#include "../EffectsLibrary.h"
#include "EffectActivity.h"
#include "../Utils/List.h"
#include "../Views/Header.h"
#include "../Preset.h"

class AddEffectActivity : public Activity
{
public:
    List<Header*> effects = List<Header*>();

    AddEffectActivity(Preset * currentPreset) : Activity()
    {
        for(int i = 0; i < effectsLibrary.library.size(); i++)
        {
            Header * hdr = new Header(effectsLibrary.library[i].name);
            hdr->onSelectEvent([=]()
            { 
                auto effect = effectsLibrary.library[i].constructor();
                currentPreset->add(effect);
                pedal.exitActivity(); 
                pedal.intent(new EffectActivity(effect));
            });
            effects.add(hdr);
            add((*hdr));
        }
    }

    ~AddEffectActivity()
    {
        for(int i = 0; i < effects.size(); i++)
        {
            delete effects[i];
        }
    }
};