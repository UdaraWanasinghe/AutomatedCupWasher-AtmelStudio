
/*
 * keypad.c
 *
 * Created: 10/15/2017 10:35:18 PM
 *  Author: Udara Wanasinghe
 */ 

#include <avr/delay.h>
#include "keypad.h"

int r, c;

//key map
char key_map[4][4] = {
	{'A', 'B', 'C', '0'},
	{'D', 'E', 'F', '0'},
	{'0', '0', '0', '0'},
	{'0', '0', '0', '0'}
};

//initialize keypad
void keypad_init(){
	KEYPAD_PORT = 0x0F; //columns 0, rows 1
}

//scan for key once
char scan_for_key(){
	for (c = 0; c < 4; c++)
	{
		//one column output and 0 state, other columns input and tristate, shift by c times
		//rows output and pull up resistors activated, so at 1 state
		KEYPAD_DDR = 0x10 << c;
		
		for (r = 0; r < 4; r++)
		{
			_delay_ms(.000001); //delay is essential to work properly
			if (!(KEYPAD_PIN & (0x01 << r))) //check column output is in row input
			{
				return key_map[r][c];
			}
		}
	}
	return 0xFF;
}