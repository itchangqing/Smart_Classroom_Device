#include "gpio.h"
#include "tim.h"
#include "fan.h"


static uint8_t sudu = 0;
static uint8_t max = 0;


void FAN_Init(void) {

	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 0);
	
	//初始化正转  gpioc pin4 控制正反转
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);

}



void FAN_Open_1(void) {
	
	if(sudu == 0) {
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 450);

		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 250);
	
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 166);
		
	}
	else __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 333);
	
	max = 1;
}


//
void FAN_Open_2(void) {
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 250);
	
	max = 2;
}


//
void FAN_Open_3(void) {
	
	if(sudu == 0) __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 499);
	else __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 0); 
	
	max = 3;
	
}


//关闭
void FAN_Close(void) {
	
	if(sudu == 0) __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 0);
	else __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, 499); 
	
	max = 0;
	
}

//翻转方向
void FAN_Toggle(void) {
	
	//sudu == 0时候是正转
	if(sudu == 0) sudu = 1;
	else sudu = 0; 
	
	//判断反转时候的挡位
	if(max == 0)FAN_Close();
	else if(max == 1)FAN_Open_1();
	else if(max == 2)FAN_Open_2();
	else FAN_Open_3();
	
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_4);
}
	

