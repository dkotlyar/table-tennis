/*
 * eeprom.c
 *
 * Created: 07.06.2016 16:32:52
 *  Author: Dmitriy
 */ 

#include "main.h"

void eeprom_write(uint16_t addr, uint8_t data) {
	while ((EECR & EEWE));
		
	EEDR = data;
	EEAR = addr;
	
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

uint8_t eeprom_read(uint16_t addr) {
	while ((EECR & EEWE));
	
	EEAR = addr;
	EECR |= (1<<EERE);
	
	return EEDR;
}