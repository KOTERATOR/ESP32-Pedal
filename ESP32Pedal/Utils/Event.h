#pragma once

#include "List.h"

template<typename ...T>
using EventHandler = void (*)(T...);

template<typename ...T>
class Event
{
  public:
    List<EventHandler<T...>> handlers;
    Event()
    {
    }

    void operator+=(EventHandler<T...> right)
    {
        //if(!handlers.contains(right))
        handlers.add(right);
    }

    void operator-=(EventHandler<T...> right)
    {
        handlers.remove(right);
    }

    void operator()(T ...args)
    {

        invoke(args...);
    }

    void invoke(T ...args)
    {
        for (size_t i = 0; i < handlers.size(); i++)
        {
            // invoking all handlers
            handlers[i](args...);
        }
    }
};
