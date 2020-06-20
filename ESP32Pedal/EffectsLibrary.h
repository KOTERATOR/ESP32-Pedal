#pragma once

#include "Utils/List.h"
#include <functional>
#include "EffectsUnit.h"
#include "Effects/Effects.h"

typedef std::function<EffectsUnit*()> EffectConstructor;

class EffectsLibraryNode
{
public:
    String name;
    EffectConstructor constructor;
    EffectsLibraryNode(String name, EffectConstructor ctr)
    {
        this->name = name;
        this->constructor = ctr;
    }
    EffectsLibraryNode() {}
};

class EffectsLibrary
{
protected:
    
public:
    List<EffectsLibraryNode> library;
    EffectsLibrary()
    {
        add("DELAY", [](){ return new Delay(); });
        add("FUZZ", [](){ return new Fuzz(); });
        add("OCTAVER", [](){ return new Octaver(); });
        add("CHRORUS", [](){ return new Chorus(); });
        add("PITCHER", [](){ return new Pitcher(); });
    }

    void add(String name, EffectConstructor ctr)
    {
        library.add(EffectsLibraryNode(name, ctr));
    }

    EffectsUnit * get(String name)
    {
        for(int i = 0; i < library.size(); i++)
        {
            if(name == library[i].name)
            {
                return library[i].constructor();
            }
        }
        return nullptr;
    }
} effectsLibrary;