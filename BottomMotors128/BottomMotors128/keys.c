/*
 * keys.c
 *
 * Created: 07.06.2016 13:58:51
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "keys.h"
#include "led.h"

#include "ultrasonic.h"

uint16_t prev_key = KEY_ZERO;
uint16_t counter = 0;


void key_init(void) {
	KEY_PORT	&= ~(1<<KEY_PIN);
	KEY_PUD		|= (1<<KEY_PIN);
}

void key_handle(void) {	
	uint16_t key_image = read_key_image(); //—читывание линий
	
	if ((prev_key == key_image) && (key_image != KEY_ZERO))
	{
		switch (counter)
		{
			case MULTLONG_DELAY:
				multiple_long_key_press(key_image);
				break;
			case ONCELONG_DELAY:
				once_long_key_press(key_image);
				counter++;
				break;
			case SHORT_DELAY:
				short_key_press(key_image);
				counter++;
				break;
			default:
				counter++;
				return;
		};
	}
	else
	{
		prev_key = key_image;
		counter = 0;
	}
}

uint16_t read_key_image(void) {
	return KEY_PORT & (1<<KEY_PIN);
}

void multiple_long_key_press(uint16_t key_image) {
	
}

void once_long_key_press(uint16_t key_image) {
	
}

void short_key_press(uint16_t key_image) {
	if (!(key_image & (1<<KEY_PIN))) {
		ultrasonic_enable();
	}
}