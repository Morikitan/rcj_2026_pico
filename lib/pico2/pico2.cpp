#include <stdio.h>
#include "pico2.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../config.hpp"
#include "pio_spi.h"
#include "hardware/pio.h"
#include "pico/binary_info.h"

uint offset;


void Pico2Setup(){
    //gpio_init(SPI_CSpin); 
    //gpio_init(SPI_SCKpin);
    //gpio_init(SPI_RXpin);
    //gpio_init(SPI_TXpin);
    //gpio_set_dir(SPI_CSpin,GPIO_IN);
    //gpio_set_function(SPI_SCKpin,GPIO_FUNC_SPI);
    //gpio_set_function(SPI_RXpin,GPIO_FUNC_SPI);
    //gpio_set_function(SPI_TXpin,GPIO_FUNC_SPI);
    //spi_init(spi0,2000000); //2.0MHz
    //spi_set_slave(spi0, true);

    gpio_set_dir(SPI_TXpin,GPIO_OUT);

    // スレーブとして使う PIO / SM / CS pin の定義
    pio_spi_inst_t spi = {
        .pio = pio0,
        .sm = 0,
        .cs_pin = SPI_CSpin
    };

    // PIO プログラムをロード
    offset = pio_add_program(spi.pio, &spi_cpha0_program);

    // PIO SPI 初期化
    pio_spi_init(spi.pio, spi.sm, offset,
                 8,        // ビット幅：8 ビット
                 15.625f,   // SPI クロック 2 MHz @ 125 MHz sysclk
                 false,    // CPHA = 0
                 false,    // CPOL = 0
                 SPI_SCKpin,
                 SPI_TXpin,  // マスターから見て MISO
                 SPI_RXpin  // マスターから見て MOSI
    );

    bi_decl(bi_4pins_with_names(
        SPI_RXpin, "MOSI",
        SPI_TXpin, "MISO",
        SPI_SCKpin, "SCK",
        spi.cs_pin, "CS"));
}

void UseSPI(){
        // 1. 受信データを読み取る（blocking：データが来るまで待つ）
        uint32_t received = pio_sm_get_blocking(pio0, 0) & 0xFF; // 8ビットのみ取り出し

        // 2. 受信データに応じた応答データを選ぶ
        uint8_t response;
        switch (received) {
            case 0x01:
                response = 0xAB;
                break;
            case 0x02:
                response = 0xCD;
                break;
            default:
                response = 0xFF;
                break;
        }

        // 3. 応答データを送信（TX FIFOに書き込む）
        pio_sm_put_blocking(pio0, 0, response);
}





