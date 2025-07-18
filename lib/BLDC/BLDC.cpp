#include <stdio.h>
#include "BLDC.hpp"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../config.hpp"

uint slice_num;
uint channel;

void BLDCSetup(){
    gpio_set_function(ESCpin, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(ESCpin);
    channel = pwm_gpio_to_channel(ESCpin);

     // ラップ値を20,000に設定（1usごとにカウント → 20ms周期）
    pwm_set_wrap(slice_num, 20000);

    // PWMクロックを設定
    pwm_set_clkdiv(slice_num, 125.0f);
    // パルス幅（1ms～2ms）
}

//pulse_usは1000～2000
void BLDCState(unsigned int pulse_us){
    // 20ms周期 -> 50Hz
    // Pico の PWM は sys_clk（125MHz）から分周して使う
    // 1μs単位 = 1MHz にしたいので125分周（125MHz ÷ 125 = 1MHz）

    
    if (pulse_us < 1000) pulse_us = 1000;
    if (pulse_us > 2000) pulse_us = 2000;
    pwm_set_chan_level(slice_num, channel, pulse_us);

    pwm_set_enabled(slice_num, true);
}