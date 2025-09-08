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
    gpio_init(SPI_CSpin); 
    gpio_init(SPI_SCKpin);
    gpio_init(SPI_RXpin);
    gpio_init(SPI_TXpin);
    gpio_set_dir(SPI_CSpin,GPIO_IN);
    gpio_set_function(SPI_SCKpin,GPIO_FUNC_SPI);
    gpio_set_function(SPI_RXpin,GPIO_FUNC_SPI);
    gpio_set_function(SPI_TXpin,GPIO_FUNC_SPI);
    spi_init(spi0,2000000); //2.0MHz
    spi_set_slave(spi0, true);

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
                 62.5f,   // SPI クロック 1 MHz @ 125 MHz sysclk
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



