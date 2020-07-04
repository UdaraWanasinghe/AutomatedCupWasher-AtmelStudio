
/*
 * bluetooth.c
 *
 * Created: 10/15/2017 1:26:41 PM
 *  Author: Udara Wanasinghe
 */ 

#include "bluetooth.h"
#include <avr/io.h>

// function to initialize UART communication
void bluetooth_init (){
	UBRRH |= (12 >> 8);										// set UBBR value for 9600 baud rate
	UBRRL |= 12;
	UCSRB |= (1 << TXEN) | (1 << RXEN);						// enable receiver and transmitter
	UCSRC &= ~(1 << UMSEL);									// enable asynchronous mode
	UCSRC|= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);		// 8bit data format
	UCSRA = 1 << U2X;										// double speed mode
	UCSRB |= (1 << RXCIE);									// enable receive complete interrupt
}

// function to transmit character
void bluetooth_transmitc (char c)
{
	while (!( UCSRA & (1<<UDRE)));                // wait while register is free
	UDR = c;                                   // load data in the register
}

// function to transmit string
void bluetooth_transmits (char *str)
{
	int i;
	for (i = 0; ;i++)
	{
		if (str[i] == '\0') break;
		bluetooth_transmitc(str[i]);	// load data in the register
	}
}

// function to receive character
char bluetooth_receivec ()
{
	while(!(UCSRA) & (1<<RXC));                   // wait while data is being received
	return UDR;                                   // return 8-bit data
}