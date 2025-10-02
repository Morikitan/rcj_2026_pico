#include <stdio.h>
#include "rp2350.hpp"
#include "pico/stdlib.h"
#include "../config.hpp"
#include "hardware/pio.h"
#include "picoPioUart.pio.h"
#include "hardware/clocks.h"

static const uint8_t response1[8] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
static const uint8_t response2[8] = { 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18};

// グローバル変数
volatile uint8_t command = 0;
volatile bool command_received = false;

PIO pio;
uint sm;
uint offset;
uint sm_rx;
uint sm_tx;
uint offset2;
bool parity_check;
    
//pioをつかったUARTの初期設定
void RP2350Setup(){
    /*********************************
    UART通信の信号
    0x01 : エンコーダー

    */
    pio = pio0;

    sm_rx = 0;

    offset = pio_add_program(pio, &picoPioUartRx_program);
    picoPioUartRx_program_init(pio, sm_rx, offset, UART_RXpin, SERIAL_BAUD);

    // 使うSMを指定します(送信と受信では別のSMを使う)
    sm_tx = 1;

    offset2 = pio_add_program(pio, &picoPioUartTx_program);
    picoPioUartTx_program_init(pio, sm_tx, offset2, UART_TXpin, SERIAL_BAUD);

    gpio_set_irq_enabled_with_callback(UART_RXpin,GPIO_IRQ_EDGE_FALL,true,&RP2350Callback);
}

//割り込みが起きたときにrp2350にデータを返す関数
void RP2350Callback(uint gpio, uint32_t events){
    picoPioUartRx_program_clear_buffer();
    unsigned char data = picoPioUartRx_program_getc(true,&parity_check);
    if(data == 0x24){
        //エンコーダー
        for(int i = 0;i < 8;i++){
            picoPioUartTx_program_putc(encoderData[i],true);
        }
    }else{

    }
}

//UART(シリアル通信)で送信する関数
//
//data : 送るデータ(uint8_t型)
//even_parity : 偶数か奇数のどちらになるようにパリティを付加するか。trueで偶数。falseで奇数。
void picoPioUartTx_program_putc(unsigned char data, bool even_parity) {
    uint32_t byte = (uint32_t)data;
    uint8_t parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= byte & 0x1;
        byte >>= 1;
    }
    byte = (uint32_t)data;
    if (parity) {
        if (even_parity) {
            byte |= 0x100;  // 偶数になるようにパリティを付加します
        }
    } else {
        if (!even_parity) {
            byte |= 0x100;  // 奇数になるようにパリティを付加します
        }
    }
    pio_sm_put_blocking(pio, sm_tx, (uint32_t)byte);  // TX FIFOへputします
}

//UART(シリアル通信)で受信する関数
//
//
//even_parity : 偶数か奇数のどちらになるようにパリティを付加されているか。trueで偶数。falseで奇数。
//parity_check : パリティビットの結果。正しいならtrue。違ったらfalseで、例外処理を用意する。データがなくてもfalseになる。
unsigned char picoPioUartRx_program_getc(bool even_parity,bool* parity_check) {
    while (pio_sm_is_rx_fifo_empty(pio, sm_rx)) tight_loop_contents();

    uint32_t c32 = pio_sm_get(pio, sm_rx);
    
    //パリティビットの検証をする
    bool real_parity = (c32 & 0x100) != 0;
    uint8_t byte = c32 & 0xff;

    uint8_t pcheck = 0;
    for (int i = 0; i < 8; i++) {
        pcheck ^= byte & 0x1;
        byte >>= 1;
    }

    *parity_check = (pcheck == real_parity);

    return (uint8_t)(c32 & 0xff);
}

void picoPioUartRx_program_clear_buffer(){
    while (!pio_sm_is_rx_fifo_empty(pio, sm_rx)){
        uint32_t c32 = pio_sm_get(pio, sm_rx);
    }
}

