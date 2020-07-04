
/*
 * motor.h
 *
 * Created: 10/15/2017 11:17:24 AM
 *  Author: Udara Wanasinghe
 */ 
#include <avr/io.h>

#define MOTOR_CLOCKWISE			1
#define MOTOR_ANTICLOCKWISE		0

#define MOTOR_DDR			DDRC
#define MOTOR_PORT			PORTC
#define MAIN_MOTOR_PIN1		PINC1
#define MAIN_MOTOR_PIN2		PINC2
#define SYRINGE_MOTOR_PIN1	PINC3
#define SYRINGE_MOTOR_PIN2	PINC4

extern void motor_init();

extern void start_main_motor(int direction);

extern void start_syringe_motor(int direction);

extern void stop_main_motor();

extern void stop_syringe_motor();