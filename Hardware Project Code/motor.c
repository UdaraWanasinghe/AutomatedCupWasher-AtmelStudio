
/*
 * motor.c
 *
 * Created: 10/15/2017 11:17:03 AM
 *  Author: Udara Wanasinghe
 */ 

#include "motor.h"

void motor_init(){
	//set motor pins as output
	MOTOR_DDR |= (1 << MAIN_MOTOR_PIN1) | (1 << MAIN_MOTOR_PIN2) | (1 << SYRINGE_MOTOR_PIN1) | (1 << SYRINGE_MOTOR_PIN2);
}

void start_main_motor(int direction){
	if (direction == MOTOR_CLOCKWISE)
	{
		MOTOR_PORT |= (1 << MAIN_MOTOR_PIN1);	// pin 1 high
		MOTOR_PORT &= ~(1 << MAIN_MOTOR_PIN2);	//pin 2 low

	}else if (direction == MOTOR_ANTICLOCKWISE)
	{
		MOTOR_PORT &= ~(1 << MAIN_MOTOR_PIN1);	// pin 1 low
		MOTOR_PORT |= (1 << MAIN_MOTOR_PIN2);	//pin 2 high
	}
}

void stop_main_motor(){
	MOTOR_PORT &= ~((1 << MAIN_MOTOR_PIN1) | (1 << MAIN_MOTOR_PIN2));	//set pin1 & pin2 low

}

void start_syringe_motor(int direction){
	if (direction == MOTOR_CLOCKWISE)
	{
		MOTOR_PORT |= (1 << SYRINGE_MOTOR_PIN1);	// pin 1 high
		MOTOR_PORT &= ~(1 << SYRINGE_MOTOR_PIN2);	//pin 2 low

	}else if (direction == MOTOR_ANTICLOCKWISE)
	{
		MOTOR_PORT &= ~(1 << SYRINGE_MOTOR_PIN1);	// pin 1 low
		MOTOR_PORT |= (1 << SYRINGE_MOTOR_PIN2);	//pin 2 high
	}
}

void stop_syringe_motor(){
	MOTOR_PORT &= ~((1 << SYRINGE_MOTOR_PIN1) | (1 << SYRINGE_MOTOR_PIN2));	//set pin1 & pin2 low

}