#pragma once

#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "driver/timer.h"

class HardwareTimer
{
private:
    static intr_handle_t s_timer_handle;

    
    void (*handler)(void*) = nullptr;
public:
    HardwareTimer(int microsecondsPeriod, void(*handler)(void*), bool repeat = true);
    void enable();
    void disable();
    void onTick(void (*handler)(void*));
};

HardwareTimer::HardwareTimer(int microsecondsPeriod, void(*handler)(void*), bool repeat)
{
    timer_config_t config = {
        .alarm_en = true,
        .counter_en = false,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = true,
        .divider = 80   /* 1 us per tick */
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, microsecondsPeriod);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, handler, NULL, 0, &s_timer_handle);

    timer_start(TIMER_GROUP_0, TIMER_0);
}

intr_handle_t HardwareTimer::s_timer_handle;

void HardwareTimer::enable()
{

}

void HardwareTimer::disable()
{

}

void HardwareTimer::onTick(void (*handler)(void*))
{
    this->handler = handler;
}