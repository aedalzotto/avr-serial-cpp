/**
 * @file Serial.cpp
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (C) 2018 by Angelo Elias Dalzotto
 * 
 * @brief This is an interrup-driven Serial library. The configurations are
 * written in C++ but it supports the stdio functions.
 * 
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Serial.h>

#ifndef SERIAL_RX_BUF_SIZE
#define SERIAL_RX_BUF_SIZE 64
#endif

#ifndef SERIAL_TX_BUF_SIZE
#define SERIAL_TX_BUF_SIZE 64
#endif

Ringbuffer Serial::txbuf(SERIAL_TX_BUF_SIZE);
Ringbuffer Serial::rxbuf(SERIAL_RX_BUF_SIZE);

ISR(SERIAL_UDRE_vect)
{
    if(Serial::txbuf.is_empty()){
        UCSR0B &= ~(1 << UDRIE0);
        return;
    }

    UDR0 = Serial::txbuf.read();
    if(Serial::txbuf.is_empty())
        UCSR0B &= ~(1 << UDRIE0);
}

ISR(SERIAL_RX_vect)
{
    Serial::rxbuf.write(UDR0);
}

int Serial::uputc(char c, FILE *stream) {
    if(c == '\n')
        uputc('\r', stream);
    
    while(txbuf.is_full());
    txbuf.write((uint8_t)c);
    
    UCSR0B |= (1 << UDRIE0);

    return 0;
}

int Serial::ugetc(FILE *stream)
{
    while(rxbuf.is_empty());
    return rxbuf.read();
}

bool Serial::init(uint32_t baud, bool doublespd)
{
    uint8_t divspd = 16;
    if(doublespd){
        UCSR0A |= (1 << U2X0);
        divspd = 8;
    }

    uint16_t ubrr_val = F_CPU / divspd / baud - 1;
    
    UBRR0H = (uint8_t)(ubrr_val >> 8);
    UBRR0L = (uint8_t)ubrr_val;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | (1 << RXCIE0);   /* Enable RX and TX */

    stdout = fdevopen(&(Serial::uputc), NULL);
    stdin  = fdevopen(NULL, &(Serial::ugetc));

    sei();

    return true;
}

uint16_t Serial::available()
{
    return rxbuf.available();
}