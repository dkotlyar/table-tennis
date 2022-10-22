/*
 * eeprom.h
 *
 * Created: 07.06.2016 16:33:02
 *  Author: Dmitriy
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void eeprom_write(uint16_t addr, uint8_t data);
uint8_t eeprom_read(uint16_t addr);


#endif /* EEPROM_H_ */