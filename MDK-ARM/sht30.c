#include "sht30.h"
#include "i2c.h"



uint8_t SHT30_Get(float* temp, float* humi) {
	
	uint8_t data[6] = {0};

	uint8_t cmd[2] = {0x24,0x00};//禁止时钟拉伸，需要延迟来完成读取
	
	HAL_I2C_Master_Transmit(&hi2c1,0x44 << 1,cmd,sizeof(cmd),1000); //发送采样命令
	
	HAL_Delay(20);
	
	HAL_I2C_Master_Receive(&hi2c1,0x44 << 1,data,sizeof(data),1000);//接收温湿度数据
	
	//SHT30 传回的温度是 16 位数据
	if(temp != NULL) *temp = -45 + 175.0 * ((uint16_t)(data[0] << 8) + data[1]) / 65535;
	
	if(humi != NULL) *humi = 100.0 * ((uint16_t)(data[3] << 8) + data[4]) / 65535;
	
	return 0;

}








