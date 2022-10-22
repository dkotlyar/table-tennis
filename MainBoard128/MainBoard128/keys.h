/*
 * keys.h
 *
 * Created: 07.06.2016 13:58:57
 *  Author: Dmitriy
 */ 


#ifndef KEYS_H_
#define KEYS_H_

enum key_codes {
	ZERO = 0
};

void key_init(void);
void key_handle(void);
uint16_t read_key_image(void);

void long_key_press(uint16_t key_image);
void short_key_press(uint16_t key_image);

#endif /* KEYS_H_ */