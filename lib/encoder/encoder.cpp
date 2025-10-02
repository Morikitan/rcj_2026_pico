#include <stdio.h>
#include "encoder.hpp"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "../config.hpp"

uint8_t data[16];
uint32_t deltaTime[4];

void EncoderSetup(){
    gpio_init(Encoder_RXpin);
    gpio_init(Encoder_TXpin);
    gpio_set_function(Encoder_RXpin,GPIO_FUNC_UART);
    gpio_set_function(Encoder_TXpin,GPIO_FUNC_UART);
    uart_init(encoderUART,230400);

    bool isMotorClockWise[4];
    float motorFrequency[4];
    uint8_t encoderData[8];
}

void UseEncoder(){
    uart_write_blocking(encoderUART,(uint8_t[]){0x03},1);
    uart_read_blocking(encoderUART,data,16);
    for(int i = 0;i < 4;i++){
        isMotorClockWise[i] = ((data[i*4] >> 7) == 1);
        data[i*4] &= 0b01111111; //最上位ビットを0にする
        deltaTime[i] = ((uint32_t)data[i*4] << 24) | ((uint32_t)data[i*4 + 1] << 16) 
        | ((uint32_t)data[i*4 + 2] << 8) | (uint32_t)data[i*4 + 3];
        if(deltaTime[i] == 0){
            motorFrequency[i] = 0;
        }else{
            //deltaTime[i]は1/4周期にかかる時間(μs)。
            motorFrequency[i] = 0.00000025 / deltaTime[i]; 
        }

        uint32_t a = motorFrequency[i] * 100;
        //小数点以下2桁までのデータを送る。
        //タイヤの回転数が80回転/秒を超えることはないだろうという前提でのプログラム
        encoderData[i*2] = ((uint8_t)(a >> 8)) | ((uint8_t)i << 5) | (isMotorClockWise[i] ? 0b00010000 : 0);
        encoderData[i*2 + 1] = (uint8_t)a;
    }
    
}
    