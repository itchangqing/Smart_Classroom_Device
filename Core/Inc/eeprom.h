#ifndef _EEPROM_H_
#define _EEPROM_H_

#define EEPROM_ADDR 0x50 << 1


#include "stdint.h"


uint8_t EEPROM_Write(uint8_t addr, uint8_t data);
uint8_t EEPROM_Read(uint8_t addr, uint8_t* data);


#endif
