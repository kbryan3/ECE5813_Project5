 /********************************************************************
*
* @file uart.c
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
#include "uart.h"
#include <stdio.h>


struct __FILE
{
  int handle;
};

FILE __stdout;  //Use with printf
FILE __stdin;		//use with fget/sscanf, or scanf


//Retarget the fputc method to use the UART0
int fputc(int ch, FILE *f){
	while(!(UART0->S1 & UART_S1_TDRE_MASK) && !(UART0->S1 & UART_S1_TC_MASK));
	UART0->D = ch;
	return ch;
}

//Retarget the fgetc method to use the UART0
int fgetc(FILE *f){
	while(!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;
}

// Code listing 8.8, p. 231
void Init_UART0(uint32_t baud_rate) {
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
//	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((DEFAULT_SYSTEM_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

#ifdef INTERRUPT
	// Enable interrupts. Listing 8.11 on p. 234
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);
#endif

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}

/* END - UART0 Device Driver
	Code created by Shannon Strutz
	Date : 5/7/2014
	Licensed under CC BY-NC-SA 3.0
	http://creativecommons.org/licenses/by-nc-sa/3.0/
	Modified by Alex Dean 9/13/2016
*/

// Code listing 8.9, p. 233
void UART0_Transmit(uint8_t c)
{
	toggleLED(1);
#ifndef INTERRUPT
	while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
#endif

		UART0->D = c;
		log_string((uint8_t*)"Transmitted Data: ", DBUG, UART0_TRANSMIT);
}

void UART0_Transmit_Poll_NoBuff(uint8_t c) {
	while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
	UART0->D = c;
}

uint8_t UART0_Receive() {
	uint8_t c;
	toggleLED(2);
	log_string((uint8_t*)"Receiving: ", DBUG, UART0_RECEIVE);
#ifndef INTERRUPT
	while (!(UART0->S1 & UART0_S1_RDRF_MASK))
		;
#endif
		c = UART0->D;
		return c;
}

// UART0 IRQ Handler. Listing 8.12 on p. 235
#ifdef INTERRUPT
void UART0_IRQHandler(void) {
	__disable_irq();
	uint8_t ch;
	bool tx_good;
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
#ifdef ECHO
			ch = UART0_Receive();
			tx_good = 1;
			UART0->C2 |= UART_C2_TIE_MASK;
#else
#endif
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		// received a character
#ifdef ECHO
			ch = UART0_Receive();
			tx_good = 1;
			UART0->C2 |= UART_C2_TIE_MASK;
#else
#endif
	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{ // tx buffer empty
		// can send another character
#ifdef ECHO
		if(tx_good)
		{
			tx_good = 0;
			UART0_Transmit(ch);
			UART0->C2 &= ~UART0_C2_TIE_MASK;


		}
#else
/*		if (!Q_Empty(&TxQ)) {
			UART0->D = Q_Dequeue(&TxQ);
		} else {
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}*/
#endif
	}
	__enable_irq();
}
#endif
void Send_String_Poll(uint8_t * str) {
	// enqueue string
//	uint32_t state = UART0->C2 & UART0_C2_TIE_MASK;
//	uint32_t state2 = UART0->C2 & UART0_C2_RIE_MASK;
	UART0->C2 &= ~UART0_C2_TIE_MASK;
	UART0->C2 &= ~UART0_C2_RIE_MASK;
	while (*str != '\0') { // Send characters up to null terminator
		//UART0_Transmit(*str++);
		toggleLED(1);
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
					;
		UART0->D = *str++;
	}
}

/*void Send_String(uint8_t * str) {
	// enqueue string
	while (*str != '\0') { // copy characters up to null terminator
		while (Q_Full(&TxQ))
			; // wait for space to open up
		Q_Enqueue(&TxQ, *str);
		str++;
	}
	// start transmitter if it isn't already running
	if (!(UART0->C2 & UART0_C2_TIE_MASK)) {
		UART0->D = Q_Dequeue(&TxQ);
		UART0->C2 |= UART0_C2_TIE(1);
	}
}*/

void echo(CIRCBUFF* txbuff, CIRCBUFF* rxbuff)
{

	uint8_t c;
	if(rxbuff->status != FULL)
	{
		c = UART0_Receive();
		add(rxbuff, c);
	}
	if(rxbuff->status != EMPTY && txbuff->status != FULL)
	{
		c = removeItem(rxbuff);
		add(txbuff, c);
	}
	if(txbuff->status != EMPTY)
	{
		c = removeItem(txbuff);
		UART0_Transmit(c);
	}

}



// ****ARM University Program Copyright © ARM Ltd 2013*******
