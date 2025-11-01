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
        UseEncoder();
    }
}
