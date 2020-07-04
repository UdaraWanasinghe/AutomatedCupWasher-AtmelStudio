/*
 * Hardware Project Code.c
 *
 * Created: 6/20/2017 10:23:43 PM
 * Author : Udara Wanasinghe
 */ 

/**
*	PORTA0				-> Soap water level
*	PORTA1				-> Light sensor 1 (Begin)
*	PORTA2				-> Light sensor 2 (End)
*	PORTA3				-> Solenoid valve 1 (Detergent mixed)
*	PORTA4				-> Solenoid valve 2 (Fresh water)
*	PORTC1~C2			-> Main motor
*	PORTC3~C4			-> Syringe motor
*	PORTB				-> Keypad
*	PORTD0~D1			-> Bluetooth module
*	PORTC0, PORTD2~D7	-> LCD Module
*/

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <inttypes.h>

#include "sensors.h"
#include "solenoid.h"
#include "motor.h"
#include "keypad.h"
#include "bluetooth.h"
#include "lcd.h"

#define bool int
#define TRUE 1
#define FALSE 0

//delays
#define USER_WAIT_DELAY 2000
#define MESSAGE_DELAY 1500
#define HOLE_FADE_AWAY_DELAY 1200
#define CLEANING_DELAY 4000

//command flags
static volatile uint8_t SLEEP_FLAG;
static volatile uint8_t WAKE_FLAG;
static volatile uint8_t SKIP_FLAG;
static volatile uint8_t WAIT_FLAG;
static volatile uint8_t REFILL_FLAG;
static volatile uint8_t DRAIN_FLAG;

//setters
void set_sleep_flag(uint8_t sleep_flag){
	SLEEP_FLAG = sleep_flag;
}

void set_wake_flag(uint8_t wake_flag){
	WAKE_FLAG = wake_flag;
}

void set_skip_flag(uint8_t skip_flag){
	SKIP_FLAG = skip_flag;
}

void set_wait_flag(uint8_t wait_flag){
	WAIT_FLAG = wait_flag;
}

void set_refill_flag(uint8_t refill_flag){
	REFILL_FLAG = refill_flag;
}

void set_drain_flag(uint8_t drain_flag){
	DRAIN_FLAG = drain_flag;
}

//getters
uint8_t get_sleep_flag(){
	return SLEEP_FLAG;
}

uint8_t get_wake_flag(){
	return WAKE_FLAG;
}

uint8_t get_skip_flag(){
	return SKIP_FLAG;
}

uint8_t get_wait_flag(){
	return WAIT_FLAG;
}

uint8_t get_refill_flag(){
	return REFILL_FLAG;
}

uint8_t get_drain_flag(){
	return DRAIN_FLAG;
}

//Interrupt Service Routine for USART_RXC vector
ISR(USART_RXC_vect){
	char receivedData = UDR; //get byte from UDR
	
	if(receivedData == 'A'){
		set_sleep_flag(TRUE);
		
	}else if (receivedData == 'B')
	{
		if (get_sleep_flag())
		{
			set_wake_flag(TRUE);
		}

	}else if (receivedData == 'C')
	{
		set_skip_flag(TRUE);
		
	}else if (receivedData == 'D')
	{
		set_wait_flag(TRUE);
		
	}else if (receivedData == 'E')
	{
		set_refill_flag(TRUE);
		
	}else if (receivedData == 'F')
	{
		set_drain_flag(TRUE);
	}
}

//initialize machine
void initialize(){
	if (!read_sensor(LASER_SENSOR1) && !read_sensor(LASER_SENSOR2))
	{
		show_animated_message("Initializing...\nPlease wait :)");
		start_main_motor(MOTOR_CLOCKWISE);
		while(!read_sensor(LASER_SENSOR1) && !read_sensor(LASER_SENSOR2));
		stop_main_motor();
	}
}

//execute sleep command if available
bool execute_sleep_command(){
	if (get_sleep_flag())
	{
		bluetooth_transmits("RR:A");
		show_animated_message("I'm sleeping ^_^\nDon't disturb");
		
		while(get_sleep_flag()){
			scan_keypad();
			if (get_wake_flag())
			{
				set_sleep_flag(FALSE);
				set_wake_flag(FALSE);
				bluetooth_transmits("RR:B");
			}
		}
		
		show_animated_message("I'm awake :D");
		_delay_ms(MESSAGE_DELAY);
		return TRUE;
	}
	return FALSE;
}

//execute wait command if available
void execute_wait_command(){
	while (get_wait_flag())
	{
		set_wait_flag(FALSE);
		bluetooth_transmits("RR:D");
		show_animated_message("Waiting... :)");
		_delay_ms(USER_WAIT_DELAY);
		scan_keypad();
		
	}
}

//execute refill command if available
void execute_refill_command(){
	if (get_refill_flag())
	{
		set_refill_flag(FALSE);
		bluetooth_transmits("RR:E");
		show_animated_message("Refilling... :)");
		if (read_ll_sensor() < LIQUID_MAX)
		{
			start_syringe_motor(MOTOR_CLOCKWISE);
		}
		while (1)
		{
			int read = read_ll_sensor();
			if(read > LIQUID_MAX){
				stop_syringe_motor();
				show_animated_message("Refilling\ndone... :)");
				_delay_ms(MESSAGE_DELAY);
				break;
			}
			update_liquid_level(read);
			scan_keypad();
		}
	}
}

//execute drain command if available
void execute_drain_command(){
	if (get_drain_flag())
	{
		set_drain_flag(FALSE);
		bluetooth_transmits("RR:F");
		show_animated_message("Draining... :)");
		if (read_ll_sensor() > LIQUID_MIN)
		{
			start_syringe_motor(MOTOR_ANTICLOCKWISE);
		}
		while (1)
		{
			int read = read_ll_sensor();
			if(read < LIQUID_MIN){
				stop_syringe_motor();
				show_animated_message("Draining\ndone... :)");
				_delay_ms(MESSAGE_DELAY);
				break;
			}
			update_liquid_level(read);
			scan_keypad();
		}
	}
}

//send liquid level
void update_liquid_level(int level){
	char l[4];
	itoa(level, l, 10);
	bluetooth_transmits(strcat("LL:", l));
}

//scan for keypad
void scan_keypad(){
	char key = scan_for_key();
	
	if(key == 'A'){
		SLEEP_FLAG = TRUE;
		
	}else if (key == 'B')
	{
		if (SLEEP_FLAG == TRUE)
		{
			WAKE_FLAG == TRUE;
		}
		
	}else if (key == 'C')
	{
		SKIP_FLAG == TRUE;
		
	}else if (key == 'D')
	{
		WAIT_FLAG = TRUE;
		
	}else if (key == 'E')
	{
		REFILL_FLAG = TRUE;
		
	}else if (key == 'F')
	{
		DRAIN_FLAG = TRUE;
		
	}
}

//initialize micro controller
void mc_init(){
	MCUCSR |= 1 << JTD;
	MCUCSR |= 1 << JTD; //set JTAG
	_delay_ms(2000); //wait while current stabilize
	sei();				//enable the global interrupt enable flag
	sensors_init();
	solenoid_init();
	motor_init();
	keypad_init();
	bluetooth_init();
	lcd_init(LCD_DISP_ON);
	
}

bool is_cup_inserted(){
	return read_sensor(LASER_SENSOR1);
}

bool is_cup_at(int sensor){
	//light on sensor -> high voltage
	return !read_sensor(sensor);
}

bool is_at_place(int sensor){
	//light on sensor -> high voltage
	return read_sensor(sensor);
}

void show_message(char* message){
	lcd_clrscr();
	lcd_puts(message);
}

void show_animated_message(char* message){
	lcd_clrscr();
	for (int i = 0; message[i] != '\0'; i++)
	{
		lcd_putc(message[i]);
		_delay_ms(40);
	}
}

int main(void)
{
	//initialize global variables
	SLEEP_FLAG = 0;
	WAKE_FLAG = 0;
	SKIP_FLAG = 0;
	WAIT_FLAG = 0;
	REFILL_FLAG = 0;
	DRAIN_FLAG = 0;
	
	//initialize micro controller
	mc_init();
	
	show_animated_message(" Welcome ^.^");//message
	_delay_ms(MESSAGE_DELAY);
	show_animated_message("   Automated\n   Cup Washer");//message
	_delay_ms(MESSAGE_DELAY);
	
	//initialize machine
	initialize();
	
	while(1){
		/********** Procedure **********/
		
		/***** execute sleep command if available *****/
		execute_sleep_command();
		
		/***** execute refill command if available *****/
		execute_refill_command();
		
		/***** execute drain command if available *****/
		execute_drain_command();
		
		/***** update liquid level *****/
		update_liquid_level(read_ll_sensor());
		
		/***** check for cup at begin *****/
		if (!is_cup_at(LASER_SENSOR1))
		{
			show_animated_message("Please insert\na cup :)");//message
		}
		while(!is_cup_at(LASER_SENSOR1)){
			scan_keypad();
			if (execute_sleep_command())
			{
				if (!is_cup_at(LASER_SENSOR1))
				{
					show_animated_message("Please insert\na cup :)");//message
				}
			}
		}
		_delay_ms(USER_WAIT_DELAY);
		
		/***** execute wait command if available *****/
		execute_wait_command();
		
		/***** rotate motor to next *****/
		show_animated_message("Process started!\n:)");//message
		start_main_motor(MOTOR_CLOCKWISE);
		while(1){
			scan_keypad();
			if(is_at_place(LASER_SENSOR1)) break;
		}
		stop_main_motor();
		
		/***** skip process if skip command available *****/
		if (!get_skip_flag())
		{
			/***** start cleaning process one *****/
			show_animated_message("Cleaning process\none in progress!");//message
			open_valve(SOLENOID_VALVE1);
			start_syringe_motor(MOTOR_CLOCKWISE);
			_delay_ms(HOLE_FADE_AWAY_DELAY);
			stop_syringe_motor();
			_delay_ms(CLEANING_DELAY);
			close_valve(SOLENOID_VALVE1);
			show_animated_message("Process one\completed! :)");//message
			
		}else{
			bluetooth_transmits("RR:C");
			show_animated_message("Skipped cleaning\nprocess one :O");
			set_skip_flag(FALSE);
			_delay_ms(MESSAGE_DELAY);
			
		}
		
		/***** rotate motor to next *****/
		start_main_motor(MOTOR_CLOCKWISE);
		_delay_ms(HOLE_FADE_AWAY_DELAY); //wait while hole is faded away
		while(1){
			scan_keypad();
			if(is_at_place(LASER_SENSOR1)) break;
		}
		stop_main_motor();
		
		if (!get_skip_flag())
		{
			/***** start cleaning process two *****/
			show_animated_message("Cleaning process\ntwo in progress!");//message
			open_valve(SOLENOID_VALVE2);
			_delay_ms(CLEANING_DELAY);
			close_valve(SOLENOID_VALVE2);
			show_animated_message("Process two\ncompleted! :)");//message
			
		}else{
			bluetooth_transmits("RR:C");
			show_animated_message("Skipped cleaning\nprocess two :O");
			set_skip_flag(FALSE);
			_delay_ms(MESSAGE_DELAY);
		}

		/***** rotate motor to next *****/
		start_main_motor(MOTOR_CLOCKWISE);
		_delay_ms(HOLE_FADE_AWAY_DELAY); //wait while hole is faded away
		while(1){
			scan_keypad();
			if(is_at_place(LASER_SENSOR1)) break;
		}
		stop_main_motor();
		
		_delay_ms(MESSAGE_DELAY);
		show_animated_message("Cleaning\ncompleted! :)");//message
		_delay_ms(MESSAGE_DELAY);
		
		/***** wait while cup is removed *****/
		if (is_cup_at(LASER_SENSOR2))
		{
			show_animated_message("Please remove\ncup from tray :)");//message
		}
		while(is_cup_at(LASER_SENSOR2)){
			scan_keypad();
			if (execute_sleep_command())
			{
				if (is_cup_at(LASER_SENSOR2))
				{
					show_animated_message("Please remove\ncup from tray :)");//message
				}
			}
		}
		_delay_ms(USER_WAIT_DELAY);
	}
}




