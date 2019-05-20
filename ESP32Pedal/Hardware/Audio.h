#pragma once

#include "driver/adc.h"
#include "driver/dac.h"
#include "soc/sens_struct.h"

typedef enum {
    ADC_CTRL_RTC = 0,
    ADC_CTRL_ULP = 1,
    ADC_CTRL_DIG = 2,
    ADC2_CTRL_PWDET = 3,
} adc_controller_t ;

class Audio
{
private:
    portMUX_TYPE rtc_spinlock = portMUX_INITIALIZER_UNLOCKED;
    void adc_set_controller(adc_unit_t unit, adc_controller_t ctrl);
    int adc_convert(int channel);
public:
    Audio();
    void getInput(int16_t * in1, int16_t * in2);
    void output(int16_t out1, int16_t out2);
};

Audio::Audio()
{
    // setup ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc_power_on();
    adc_set_controller(ADC_UNIT_1, ADC_CTRL_RTC);
    // pin 32 & 33
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    

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
    //(*in1) = analogRead(32);
    //(*in2) = analogRead(33);
    portENTER_CRITICAL(&rtc_spinlock);
    (*in1) = adc_convert(ADC1_CHANNEL_4);
    (*in2) = adc_convert(ADC1_CHANNEL_5);
    portEXIT_CRITICAL(&rtc_spinlock);
}

void Audio::output(int16_t out1, int16_t out2)
{
    dac_output_voltage(DAC_CHANNEL_1, map(out1, 0, 4095, 0, 255));
    dac_output_voltage(DAC_CHANNEL_2, map(out2, 0, 4095, 0, 255));
}

void Audio::adc_set_controller(adc_unit_t unit, adc_controller_t ctrl )
{
    if ( unit == ADC_UNIT_1 ) {
        switch( ctrl ) {
            case ADC_CTRL_RTC:
                SENS.sar_read_ctrl.sar1_dig_force = false;      //RTC controller controls the ADC, not digital controller
                SENS.sar_meas_start1.meas1_start_force = true;  //RTC controller controls the ADC,not ulp coprocessor
                SENS.sar_meas_start1.sar1_en_pad_force = true;  //RTC controller controls the data port, not ulp coprocessor
                SENS.sar_touch_ctrl1.xpd_hall_force = true;     // RTC controller controls the hall sensor power,not ulp coprocessor
                SENS.sar_touch_ctrl1.hall_phase_force = true;   // RTC controller controls the hall sensor phase,not ulp coprocessor
                break;
            case ADC_CTRL_ULP:
                SENS.sar_read_ctrl.sar1_dig_force = false;
                SENS.sar_meas_start1.meas1_start_force = false;
                SENS.sar_meas_start1.sar1_en_pad_force = false;
                SENS.sar_touch_ctrl1.xpd_hall_force = false;
                SENS.sar_touch_ctrl1.hall_phase_force = false;
                break;
            case ADC_CTRL_DIG:
                SENS.sar_read_ctrl.sar1_dig_force = true;
                SENS.sar_meas_start1.meas1_start_force = true;
                SENS.sar_meas_start1.sar1_en_pad_force = true;
                SENS.sar_touch_ctrl1.xpd_hall_force = true;
                SENS.sar_touch_ctrl1.hall_phase_force = true;
                break;
            default:
                break;            
        }
    } else if ( unit == ADC_UNIT_2) {
        switch( ctrl ) {
            case ADC_CTRL_RTC:
                SENS.sar_meas_start2.meas2_start_force = true;  //RTC controller controls the ADC,not ulp coprocessor 
                SENS.sar_meas_start2.sar2_en_pad_force = true;  //RTC controller controls the data port, not ulp coprocessor
                SENS.sar_read_ctrl2.sar2_dig_force = false;     //RTC controller controls the ADC, not digital controller
                SENS.sar_read_ctrl2.sar2_pwdet_force = false;   //RTC controller controls the ADC, not PWDET
                break;
            case ADC_CTRL_ULP:
                SENS.sar_meas_start2.meas2_start_force = false;
                SENS.sar_meas_start2.sar2_en_pad_force = false;
                SENS.sar_read_ctrl2.sar2_dig_force = false;
                SENS.sar_read_ctrl2.sar2_pwdet_force = false;

                break;
            case ADC_CTRL_DIG:
                SENS.sar_meas_start2.meas2_start_force = true;
                SENS.sar_meas_start2.sar2_en_pad_force = true;
                SENS.sar_read_ctrl2.sar2_dig_force = true;
                SENS.sar_read_ctrl2.sar2_pwdet_force = false;

                break;
            case ADC2_CTRL_PWDET:
                //currently only used by Wi-Fi
                SENS.sar_meas_start2.meas2_start_force = true;
                SENS.sar_meas_start2.sar2_en_pad_force = true;
                SENS.sar_read_ctrl2.sar2_dig_force = false;
                SENS.sar_read_ctrl2.sar2_pwdet_force = true;

                break;
            default:
                break;            
        }
    } else {
      assert(0);
    }
}

int Audio::adc_convert(int channel)
{
    uint16_t adc_value;
   
        SENS.sar_meas_start1.sar1_en_pad = (1 << channel); //only one channel is selected.
        while (SENS.sar_slave_addr1.meas_status != 0);
        SENS.sar_meas_start1.meas1_start_sar = 0;
        SENS.sar_meas_start1.meas1_start_sar = 1;
        while (SENS.sar_meas_start1.meas1_done_sar == 0);
        adc_value = SENS.sar_meas_start1.meas1_data_sar;

    return adc_value;
}
