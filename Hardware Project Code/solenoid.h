
/*
 * solenoid.h
 *
 * Created: 10/15/2017 9:27:53 AM
 *  Author: Udara Wanasinghe
 */ 

#define SOLENOID_DDR		DDRA
#define SOLENOID_PORT		PORTA
#define SOLENOID_VALVE1		PINA3
#define SOLENOID_VALVE2		PINA4

extern void solenoid_init();

extern void open_valve(int valve_pin);

extern void close_valve(int valve_pin);