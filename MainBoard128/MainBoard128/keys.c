/*
 * keys.c
 *
 * Created: 07.06.2016 13:58:51
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "keys.h"
#include "led.h"

void key_init(void) {
	//DDRA |= 0x3F;
}

void key_handle(void) {	
	//uint16_t key_image = read_key_image(); //Считывание линий
	//
	//static uint16_t prev_key = ZERO; //Состояние линий в предыдущий момент времени
	//static uint8_t counter = 0;   //Счётчик удерживания кнопки
	//
	//if ((prev_key == key_image) && (key_image != ZERO))
	//{
		//switch (counter)
		//{
			////Длительное нажатие
			//case 100: 
				//long_key_press(key_image);
				//break;
			////Кратковременное нажатие break не добавлять!
			//case   5:
				//short_key_press(key_image);
			//default: 
				//counter++;
				//return;
		//};
	//}
	//else
	//{
		//prev_key = key_image;
		//counter = 0;
	//}
}

uint16_t read_key_image(void) {
	return PINA & 0x3F;
}

void long_key_press(uint16_t key_image) {
	
}

void short_key_press(uint16_t key_image) {
}