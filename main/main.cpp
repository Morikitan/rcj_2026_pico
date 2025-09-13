#include <stdio.h>
#include "pico/stdlib.h"
#include "BLDC/BLDC.hpp"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "rp2350/rp2350.hpp"
#include "config.hpp"

int main()
{
    stdio_init_all();
    /*BLDCSetup();

    BLDCState(1000);
    printf("Arming ESC...\n");
    sleep_ms(2000);
    */
    SPISetup();
    while (true) {
        /*for (int pulse = 1000;pulse <= 2000;pulse += 50){
            BLDCState(pulse);
            printf("%d\n",pulse);
            sleep_ms(300);
        }
        BLDCState(1000);
        printf("1000\n");
        sleep_ms(1000);*/
        SPISlave();
    }
}
