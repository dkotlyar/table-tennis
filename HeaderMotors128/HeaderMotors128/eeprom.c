/*
 * eeprom.c
 *
 * Created: 07.06.2016 16:32:52
 *  Author: Dmitriy
 */ 

#include "main.h"

void eeprom_write(uint16_t addr, uint8_t data) {
	while ((EECR & (1<<EEWE)));
		
	EEDR = data;
	EEAR = addr;
	
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

uint8_t eeprom_read(uint16_t addr) {
	while ((EECR & (1<<EEWE)));
	
	EEAR = addr;
	EECR |= (1<<EERE);
	
	return EEDR;
}

void eeprom_write_word(uint16_t addr, uint16_t data) {
	eeprom_write(addr, (uint8_t)(data & 0xFF));
	eeprom_write(addr+1, (uint8_t)(data>>8 & 0xFF));
}

uint16_t eeprom_read_word(uint16_t addr) {
	return ((eeprom_read(addr+1)) << 8) | eeprom_read(addr);
	
	//uint16_t result = 0;
	//result |= (eeprom_read(addr+1));
	//result <<= 8;
	//result |= eeprom_read(addr);
	//return result;
}
