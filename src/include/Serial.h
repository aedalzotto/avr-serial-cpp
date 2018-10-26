/**
 * @file Serial.h
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

#ifndef _AVR_SERIAL_H_
#define _AVR_SERIAL_H_

#include <stdio.h>
#include <avr/io.h>
#include <Ringbuffer.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATmega2560__)
    #define SERIAL_UDRE_vect USART0_UDRE_vect
    #define SERIAL_RX_vect USART0_RX_vect
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
    #define SERIAL_UDRE_vect USART_UDRE_vect
    #define SERIAL_RX_vect USART_RX_vect
#else
    #error "MCU Not supported yet, please develop and push to git!"
#endif

extern "C" void SERIAL_UDRE_vect(void) __attribute__((signal));
extern "C" void SERIAL_RX_vect(void) __attribute__((signal));

class Serial {
public:
    static bool init(uint32_t baud, bool doublepsd = true);

    static int uputc(char c, FILE *stream);
    static int ugetc(FILE *stream);

    static uint16_t available();

    friend void SERIAL_UDRE_vect(void);
    friend void SERIAL_RX_vect(void);

private:
    static Ringbuffer rxbuf;
    static Ringbuffer txbuf;

};


#endif /* _AVR_SERIAL_H_ */