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
    gpio_init(Offence_Switchpin);       gpio_set_dir(Offence_Switchpin,GPIO_IN);
    gpio_init(Start_Switchpin);         gpio_set_dir(Start_Switchpin,GPIO_IN);
    gpio_init(Determination_Switchpin); gpio_set_dir(Determination_Switchpin,GPIO_IN);
    gpio_init(Switchpin1);              gpio_set_dir(Switchpin1,GPIO_IN);
    gpio_init(Switchpin2);              gpio_set_dir(Switchpin2,GPIO_IN);
    gpio_init(Switchpin3);              gpio_set_dir(Switchpin3,GPIO_IN);

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
         * 99 : 特殊挙動
         */
        UseEncoder();
        if(mode == 0 && gpio_get(Determination_Switchpin) == true){
            if(gpio_get(Offence_Switchpin) == true){
                //攻撃
                if(gpio_get(Start_Switchpin) == true){
                    mode = 1;
                }else{
                    mode = 2;
                }
            }else{
                //防御
                if(gpio_get(Start_Switchpin) == true){
                    mode = 3;
                }else{
                    mode = 4;
                }
            }
            TellRP2350NewMode();
        }else if(mode == 1 || mode == 2){
            if(gpio_get(Offence_Switchpin) == false){
                mode += 2;
            }
            TellRP2350NewMode();
        }else if(mode == 3 || mode == 4){
            if(gpio_get(Offence_Switchpin) == true){
                mode -= 2;
            }
            TellRP2350NewMode();
        }
    }
}
