#include <stdio.h>
#include "pico/stdlib.h"
#include "BLDC/BLDC.hpp"
#include "encoder/encoder.hpp"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "rp2350/rp2350.hpp"
#include "config.hpp"
#include <string.h>

int mode = 0;
int premode = 0;
std::string SerialWatch = "ang";

int main()
{
    stdio_init_all();

    BLDCSetup();
    BLDCState(1000);
    printf("Arming ESC...\n");
    sleep_ms(2000);
    
    RP2350Setup();
    sleep_ms(100);
    
    unsigned char a = 0;
    while (true) {
        /*******************************
         * modeの対応表
         * 0 : 何もない
         * 1 : 攻撃(通常)
         * 2 : 攻撃(反転)
         * 3 : 防御(通常)
         * 4 : 防御(反転)
         */
        UseEncoder();
        
    }
}
