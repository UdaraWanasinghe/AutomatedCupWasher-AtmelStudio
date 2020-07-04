
/*
 * sensors.c
 *
 * Created: 10/15/2017 11:07:36 AM
 *  Author: Udara Wanasinghe
 */ 

#include "sensors.h"
#define bool int

void sensors_init(){
	//set laser sensor pins as input
	SENSOR_DDR &= ~((1 << LIQUID_LEVEL_SENSOR) | (1 << LASER_SENSOR1) | (1 << LASER_SENSOR2));
	adc_init();
}

//initialize ADC
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

//read ADC value of specified channel
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

bool read_sensor(int sensor_pin){
	//return pin value
	return SENSOR_PIN & (1 << sensor_pin);
}

//read from liquid level sensor
int read_ll_sensor(){
	return adc_read(LIQUID_LEVEL_SENSOR); // read ADC from channel of liquid level sensor
}