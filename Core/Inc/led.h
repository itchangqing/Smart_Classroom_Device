#ifndef _LED_H_
#define _LED_H_


#include "stdint.h"



void LED_Open(void);
void LED_Close(void);


void Set_hao(uint8_t temp);
void Set_Mode(uint8_t temp);
void Set_Tick(uint8_t temp);	

void LED_PRO_Open_1(void);
void LED_PRO_Open_2(void);	
void LED_PRO_Open_3(void);
void LED_PRO_Close(void);
void LED_PRO_Huhu(void);
void LED_PRO_Time(uint16_t t);



#endif
