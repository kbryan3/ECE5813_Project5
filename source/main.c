/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    main.c
 * @brief   Application entry point to run a Temperature Sensing Program
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "led_control.h"
#include "logger.h"
#include "circularbuffer.h"
#include "uart.h"
#include "Testsuite.h"
#include "defines.h"

const uint8_t RED = 0;
const uint8_t GREEN = 1;
const uint8_t BLUE = 2;
const uint8_t OFF = 3;

_Bool log_a;
logger_level log_level;
uint8_t c;
uint8_t app[128];

/*
 * @brief   Application entry point.
 */
int main(void)
{
//	log_level = STATUS;
#ifndef UCUNITTEST
	/* Init board hardware. */
      BOARD_InitBootPins();
//      BOARD_InitBootClocks();
      BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
//    BOARD_InitDebugConsole();
		//turn logger on and set status
		log_a = 1;
#ifdef DEBUGGING
	log_level = DBUG;
#else
//	log_level = STATUS;
#endif
	//init UART0
	Init_UART0(9600);
    initializeLEDs();
    toggleLED(OFF);
    CIRCBUFF * tx_buffer = (CIRCBUFF *)malloc(20);
    uint8_t * transmit = (uint8_t *)malloc(256);
    CIRCBUFF * rx_buffer = (CIRCBUFF *)malloc(20);
    uint8_t * receive = (uint8_t *)malloc(256);
#ifndef ECHO
    memset(app, 0, 128); //ascii values
    uint8_t ch;
#endif
    toggleLED(GREEN);

    initCircBuffer(tx_buffer, transmit, 256);
    initCircBuffer(rx_buffer, receive, 256);
	// Code listing 8.9, p. 233
	while (1) {
#ifdef ECHO
#ifndef INTERRUPT
		echo(tx_buffer, rx_buffer);
#else

#endif
#else  //Application Mode
		//wait until a non-alpha character is entered
		while (1)
		{
			ch = UART0_Receive();
			add(rx_buffer, ch);
			if(!(isalpha(rx_buffer->buffer[rx_buffer->head - 1])))
			{
				break;
			}
		}
		//total up character values
		while(rx_buffer->status != EMPTY)
		{
			c = removeItem(rx_buffer);
			app[c]++;
		}
		//transfer upper case to tx_buffer
		for(uint8_t i = 65; i<91; i++)
		{
			add(tx_buffer, i);
			add(tx_buffer,  (uint8_t)45);
			add(tx_buffer, *convert(app[i],10));
			add(tx_buffer, (uint8_t)59);
		}
		//transfer lower case to tx_buffer
		for(uint8_t i = 97; i<123; i++)
		{
			add(tx_buffer, i);
			add(tx_buffer,  (uint8_t)45);
			add(tx_buffer, *convert(app[i],10));
			add(tx_buffer, (uint8_t)59);
		}
		//print out results
		while(tx_buffer->status != EMPTY)
		{
			UART0_Transmit(removeItem(tx_buffer));
		}
#endif
	}

#else
    log_a = 1;
	log_level = TEST;
    Testsuite_RunTests();
#endif
}


