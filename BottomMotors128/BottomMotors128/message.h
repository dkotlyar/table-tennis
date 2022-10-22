/*
 * message.h
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 

#ifndef MESSAGE_H_
#define MESSAGE_H_

void add_transmit(uint8_t * data, uint8_t length);
void transmit_to0(uint8_t * data, uint8_t length);
void transmit_to1(uint8_t * data, uint8_t length);

void handle_from0(uint8_t * data, uint8_t length);
void handle_from1(uint8_t * data, uint8_t length);

#endif /* MESSAGE_H_ */