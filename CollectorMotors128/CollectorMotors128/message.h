/*
 * message.h
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef MESSAGE_H_
#define MESSAGE_H_

void add_transmit(uint8_t * data, uint8_t length);
void handle_message(uint8_t * data, uint8_t length);

#endif /* MESSAGE_H_ */