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
void RP2350Setup();
void picoPioUartTx_program_putc(unsigned char c, bool even_parity);
unsigned char picoPioUartRx_program_getc(bool even_parity,bool* parity_check);

#ifdef __cplusplus
}
#endif