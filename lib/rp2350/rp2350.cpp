#include <stdio.h>
#include "rp2350.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../config.hpp"
#include "hardware/pio.h"
#include "spi_slave.pio.h"
#include "hardware/clocks.h"

static const uint8_t response1[10] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A };
static const uint8_t response2[10] = { 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A };

// グローバル変数
volatile uint8_t command = 0;
volatile bool command_received = false;

// 状態マシン番号と PIO インスタンス
#define SM_INSTANCE    0

void SPISetup(){
    stdio_init_all();
    // 応答デバッグ用
    printf("SPI slave via PIO start\n");

    // PIO プログラムをロード
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &spi_slave_program);

    gpio_pull_up(SPI_CSpin);
    gpio_set_dir(SPI_CSpin, GPIO_IN);

    gpio_set_dir(SPI_TXpin, GPIO_IN);
    gpio_set_dir(SPI_SCKpin,  GPIO_IN);

    gpio_set_dir(SPI_RXpin, GPIO_OUT);

    // ステートマシンの初期化
    spi_slave_program_init(pio, SM_INSTANCE , offset,
                           SPI_SCKpin, SPI_RXpin, SPI_TXpin, SPI_CSpin);

    // 割り込み設定（コマンド受信の検知）
    irq_set_exclusive_handler(PIO0_IRQ_0, irq_handler);
    irq_set_enabled(PIO0_IRQ_0, true);

    // enable IRQ from PIO for SM0
    pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty, true);
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
    if (command_received) {
            command_received = false;
            // レスポンスを準備
            const uint8_t *resp;
            if (command == 0x01) {
                resp = response1;
            } else if (command == 0x02) {
                resp = response2;
            } else {
                // それ以外ならすべて 0x00
                static const uint8_t resp_zero[10] = {0};
                resp = resp_zero;
            }
            // 応答を FIFO に入れておく（10 バイト）
            for (int i = 0; i < 10; i++) {
                pio_sm_put_blocking(pio0, SM_INSTANCE, resp[i]);
            }
        }
        tight_loop_contents();
}

/*void spi_slave_program_init(PIO pio, uint sm, uint offset,
                            uint pin_sck, uint pin_mosi, uint pin_miso, uint pin_cs) {
    // ピンを入力／出力モードにセット
    gpio_pull_up(pin_cs);
    gpio_set_dir(pin_cs, GPIO_IN);

    gpio_set_dir(pin_mosi, GPIO_IN);
    gpio_set_dir(pin_sck,  GPIO_IN);

    gpio_set_dir(pin_miso, GPIO_OUT);

    // PIO ステートマシンセットアップ
    pio_sm_claim(pio, sm);

    // プログラムをロード済 (offset)

    // State machine の設定
    spi_slave_program_init_once(pio, sm, offset, pin_sck, pin_mosi, pin_miso, pin_cs);
    // ※ spi_slave_program_init_once は PIO プログラムに auto-generated による
    //    init 関数。pio_add_program すると生成されるヘルパーを使う。

    // クロック周波数設定
    // 2 MHz を目指す → ピンのディレイなどを考慮してプリスケーラを適切に
    uint32_t sys_clock = clock_get_hz(clk_sys);
    // PIO SM クロックは sys_clock／Ｔ で決まる。SCK が外部入力なので、
    // 内部のクロックは主にシフト／ステート遷移用。応答遅延や FIFO のタイミングなどが入る。
}
    */




