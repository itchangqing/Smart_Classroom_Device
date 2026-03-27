#include "eeprom.h"
#include "i2c.h"



//向指定地址存一个字节
uint8_t EEPROM_Write(uint8_t addr, uint8_t data) {

	uint8_t temp[2] = {addr,data};
	
	if( HAL_OK == HAL_I2C_Master_Transmit(&hi2c1,EEPROM_ADDR,temp,sizeof(temp),1000))
		return 0;
	
	return 1;
}


//向指定地址读取一个字节
uint8_t EEPROM_Read(uint8_t addr, uint8_t* data) {
	
	if( HAL_OK != HAL_I2C_Master_Transmit(&hi2c1,EEPROM_ADDR,&addr,sizeof(addr),1000))
		return 1;
	
	if( HAL_OK != HAL_I2C_Master_Receive(&hi2c1,EEPROM_ADDR,data,sizeof(data),1000))
		return 2;
	
	return 0;
}

