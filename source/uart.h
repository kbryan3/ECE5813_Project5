 /********************************************************************
*
* @file uart.h
* @brief UART driver for FRDM Board
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
*  Code is heavily based off of the github code linked to in lecture
*  information on the source is below:
*  BEGIN - UART0 Device Driver
*	Code created by Shannon Strutz
*	Date : 5/7/2014
*	Licensed under CC BY-NC-SA 3.0
*	http://creativecommons.org/licenses/by-nc-sa/3.0/
*	Modified by Alex Dean 9/13/2016
*	https://github.com/alexander-g-dean/ESF/tree/master/Code/Chapter_8/Serial-Demo
***********************************************************************/
#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <MKL25Z4.H>
#include "queue.h"

// 0 for polled UART communications, 1 for interrupt-driven
#define USE_UART_INTERRUPTS 	(0)
#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6)
#define SYS_CLOCK							(48e6)

void Init_UART0(uint32_t baud_rate);
void UART0_Transmit_Poll(uint8_t data);
uint8_t UART0_Receive_Poll(void);

void Send_String_Poll(uint8_t * str);
void Send_String(uint8_t * str);

uint32_t Rx_Chars_Available(void);
uint8_t	Get_Rx_Char(void);

extern Q_T TxQ, RxQ;

#endif
// *ARM University Program Copyright © ARM Ltd 2013****
