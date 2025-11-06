#pragma once


#include "hardware/pio.h"


#ifdef __cplusplus
extern "C" {
#endif

void RP2350Callback(uint gpio, uint32_t events);
void RP2350Setup();
void picoPioUartTx_program_putc(unsigned char c, bool even_parity);
unsigned char picoPioUartRx_program_getc(bool even_parity,bool* parity_check);
void picoPioUartRx_program_clear_buffer();
void GiveRP2350NewMode();

#ifdef __cplusplus
}
#endif