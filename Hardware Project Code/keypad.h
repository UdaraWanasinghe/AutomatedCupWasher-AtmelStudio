
/*
 * keypad.h
 *
 * Created		: 10/15/2017 6:59:19 PM
 * Author		: Udara Wanasinghe
 * References	: http://extremeelectronics.co.in/avr-tutorials/4x3-matrix-keypad-interface-avr-tutorial/
 */ 

#include <avr/io.h>

#define KEYPAD_DDR DDRB
#define KEYPAD_PORT PORTB
#define KEYPAD_PIN PINB

//initialize keypad
extern void keypad_init();

//scan for key once
extern char scan_for_key();