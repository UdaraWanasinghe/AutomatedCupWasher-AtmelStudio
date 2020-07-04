
/*
 * bluetooth.h
 *
 * Created: 10/15/2017 1:24:12 PM
 *  Author: Udara Wanasinghe
 */ 

#define F_CPU 1000000UL
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

// initialize UART communication
extern void bluetooth_init ();

// function to transmit character
extern void bluetooth_transmitc (char c);

// function to transmit string
extern void bluetooth_transmits (char *str);

// function to receive character
extern char bluetooth_receivec ();