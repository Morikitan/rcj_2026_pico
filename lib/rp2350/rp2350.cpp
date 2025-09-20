#include <stdio.h>
#include "rp2350.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../config.hpp"
#include "hardware/pio.h"
#include "spi_slave.pio.h"
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

// 状態マシン番号と PIO インスタンス
#define SM_INSTANCE    0

void SPISetup(){
    /*stdio_init_all();
    // 応答デバッグ用
    printf("SPI slave via PIO start\n");

    // PIO プログラムをロード

    gpio_pull_up(SPI_CSpin);
    gpio_set_dir(SPI_CSpin, GPIO_IN);

    gpio_set_dir(SPI_TXpin, GPIO_IN);
    gpio_set_dir(SPI_SCKpin,  GPIO_IN);

    gpio_set_dir(SPI_RXpin, GPIO_OUT);

    pio = pio0;
    offset = pio_add_program(pio, &spi_slave_program);
    sm = pio_claim_unused_sm(pio, true);

    // ステートマシンの初期化
    spi_slave_program_init(pio, sm , offset,
                           SPI_SCKpin, SPI_RXpin, SPI_TXpin, SPI_CSpin);

    
    // 割り込み設定（コマンド受信の検知）
    //irq_set_exclusive_handler(PIO0_IRQ_0, irq_handler);
    //irq_set_enabled(PIO0_IRQ_0, true);

    // enable IRQ from PIO for SM0
    //pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty, true);

    //spi_set_format()
    */
}

void irq_handler() {
    // PIO の受信 FIFO をチェック
    if (pio_interrupt_get(pio0, 0)) {
        // コマンドバイトを読み出し
        // FIFO に push された値を pop
        command = (uint8_t)pio_sm_get_blocking(pio0, SM_INSTANCE);
        command_received = true;
        // 割り込みクリア
        pio_interrupt_clear(pio0, 0);
    }
}

void SPISlave(){
    if (!pio_sm_is_rx_fifo_empty(pio, sm)) {
            uint8_t received = pio_sm_get(pio, sm) & 0xFF;
            //printf("Received: 0x%02X\n", received);

            if (received == 0x01) {
                for (int i = 0; i < 8; ++i) {
                    while (pio_sm_is_tx_fifo_full(pio, sm)) tight_loop_contents();
                    pio_sm_put(pio, sm, response1[i]);
                    //printf("Sent: 0x%02X\n", response1[i]);
                }
            }
        }
}

void spi_slave_program_init(PIO pio, uint sm, uint offset,
                            uint pin_sck, uint pin_mosi,
                            uint pin_miso, uint pin_cs) {
    /*pio_sm_config c = spi_slave_program_get_default_config(offset);

    sm_config_set_in_pins(&c, pin_mosi);          // MOSI入力
    sm_config_set_sideset_pins(&c, pin_miso);     // MISO出力
    sm_config_set_clkdiv(&c, 1.0f);               // クロック分周

    // GPIO初期化
    pio_gpio_init(pio, pin_sck);
    pio_gpio_init(pio, pin_mosi);
    pio_gpio_init(pio, pin_miso);
    pio_gpio_init(pio, pin_cs);

    // ピン方向設定
    pio_sm_set_consecutive_pindirs(pio, sm, pin_miso, 1, true);  // MISOは出力
    pio_sm_set_consecutive_pindirs(pio, sm, pin_mosi, 1, false); // MOSIは入力
    pio_sm_set_consecutive_pindirs(pio, sm, pin_sck,  1, false); // SCKは入力
    pio_sm_set_consecutive_pindirs(pio, sm, pin_cs,   1, false); // CSは入力

    // 状態マシン初期化・有効化
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
    */
}
    
//pioをつかったUARTの初期設定
void RP2350Setup(){
    /*********************************
    UART通信の信号
    0x01 : エンコーダー

    */
    pio = pio0;

    sm_rx = 0;

    offset = pio_add_program(pio, &picoPioUartRx_program);
    picoPioUartRx_program_init(pio, sm_rx, offset, SPI_RXpin, SERIAL_BAUD);

    // 使うSMを指定します(送信と受信では別のSMを使う)
    sm_tx = 1;

    offset2 = pio_add_program(pio, &picoPioUartTx_program);
    picoPioUartTx_program_init(pio, sm_tx, offset2, SPI_TXpin, SERIAL_BAUD);
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
    if(pio_sm_is_rx_fifo_empty(pio, sm_rx)){
        *parity_check = false;
        return 0;
    }else{
    //while (pio_sm_is_rx_fifo_empty(pio, sm_rx)) tight_loop_contents();

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
}

