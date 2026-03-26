#ifndef _LED_PRO_H_
#define _LED_PRO_H_


#include "stdint.h"

void Set_hao(uint8_t temp);
void Set_Mode(uint8_t temp);
void Set_Tick(uint8_t temp);	
void Get_Every(uint8_t* temp1,uint8_t* temp2,uint8_t* temp3);
	
void LED_PRO_Open(uint8_t dangwei);
void LED_PRO_Mode(uint8_t mode);
void LED_PRO_Time(uint16_t t);



#endif