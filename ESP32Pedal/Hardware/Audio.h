#pragma once

#include "driver/adc.h"
#include "driver/dac.h"

class Audio
{
public:
    Audio();
    void getInput(int16_t * in1, int16_t * in2);
    void output(int16_t out1, int16_t out2);
};

Audio::Audio()
{
    // setup ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    // pin 32 & 33
    adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_11);

    // setup DAC
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    dac_output_voltage(DAC_CHANNEL_1, 0);
    dac_output_voltage(DAC_CHANNEL_2, 0);
}

void Audio::getInput(int16_t * in1, int16_t * in2)
{
    //(*in1) = adc1_get_raw(ADC1_CHANNEL_4);
    //(*in2) = adc1_get_raw(ADC1_CHANNEL_5);
    (*in1) = analogRead(32);
    (*in2) = analogRead(33);
}

void Audio::output(int16_t out1, int16_t out2)
{
    dac_output_voltage(DAC_CHANNEL_1, map(out1, 0, 4095, 0, 255));
    dac_output_voltage(DAC_CHANNEL_2, map(out2, 0, 4095, 0, 255));
}

