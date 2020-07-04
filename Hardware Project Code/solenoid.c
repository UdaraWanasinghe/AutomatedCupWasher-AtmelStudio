
/*
 * solenoid.c
 *
 * Created: 10/15/2017 9:27:42 AM
 *  Author: Udara Wanasinghe
 */ 

#include <avr/io.h>
#include "solenoid.h"

void solenoid_init(){
	//set solenoid pins as output
	SOLENOID_DDR |= (1 << SOLENOID_VALVE1) | (1 << SOLENOID_VALVE2);
}

void open_valve(int valve_pin){
	//set valve pin 1
	SOLENOID_PORT |= (1 << valve_pin);
}

void close_valve(int valve_pin){
	//set valve pin 0
	SOLENOID_PORT &= ~(1 << valve_pin);
}