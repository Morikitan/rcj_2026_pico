#pragma once


#include "hardware/pio.h"


#ifdef __cplusplus
extern "C" {
#endif

void SPISetup();
void irq_handler();
void SPISlave();
void spi_slave_program_init(PIO pio, uint sm, uint offset,
                            uint pin_sck, uint pin_mosi, uint pin_miso, uint pin_cs);

#ifdef __cplusplus
}
#endif