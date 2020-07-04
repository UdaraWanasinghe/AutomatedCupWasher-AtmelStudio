
/*
 * sensors.h
 *
 * Created: 10/15/2017 11:07:21 AM
 *  Author: Udara Wanasinghe
 */ 

#include <avr/io.h>
#define bool int

//configurations
#define SENSOR_DDR				DDRA
#define SENSOR_PIN				PINA
#define LIQUID_LEVEL_SENSOR		PINA0
#define LASER_SENSOR1			PINA1
#define LASER_SENSOR2			PINA2

#define LIQUID_MAX 1000
#define LIQUID_MIN 100

//functions
extern void sensors_init();

//read from sensor
extern bool read_sensor(int sensor_pin);

//read from liquid level sensor
extern int read_ll_sensor();