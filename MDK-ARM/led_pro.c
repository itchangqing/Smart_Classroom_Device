#include <math.h>
#include "led_pro.h"
#include "tim.h"

static uint8_t	LED_Pro_hao = 0;	//挡位
static uint8_t	LED_Pro_mode = 0;	//呼吸或者别的模式
static uint8_t	LED_Pro_tick = 0;	//定时


//智能台灯的控制,通过全局变量不同去执行不同的函数
void Set_hao(uint8_t temp) {
	LED_Pro_hao = temp;
	LED_Pro_mode = 0;	//呼吸或者别的模式
	LED_Pro_tick = 0;
}

void Set_Mode(uint8_t temp) { //一次中断只能进行一个模式，所以其他的模式必须是0
	LED_Pro_mode = temp;
	LED_Pro_hao = 0;
	LED_Pro_tick = 0;
}

void Set_Tick(uint8_t temp) {
	LED_Pro_tick = temp;
	LED_Pro_hao = 0;	
	LED_Pro_mode = 0;
}

void Get_Every(uint8_t* temp1,uint8_t* temp2,uint8_t* temp3) {
	*temp1 = LED_Pro_hao;
	*temp2 = LED_Pro_mode;
	*temp3 = LED_Pro_tick;
}

//智能台灯的控制
void LED_PRO_Open(uint8_t dangwei) {
	
	if(dangwei == 0) {
		
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);//动态写入ccr
	
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
		
	
	}
	else if(dangwei == 1) {
	
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,166);//动态写入ccr
	}
	else if(dangwei == 2) {
		
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,250);//动态写入ccr
	}
	else {
		
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,499);//动态写入ccr
	}
	
}


//呼吸灯
void LED_PRO_Mode(uint8_t mode) {
	
	if(mode == 1) {
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		
		float t =  HAL_GetTick() * 0.001; //转换成秒
		
		float duty = 0.5 * sin(2 * 3.14 * t) + 0.5; //随秒而变化
		
		uint16_t ccr = duty * 500;//得到此时的ccr
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,ccr);//动态写入ccr
	}
	
}



//定时
void LED_PRO_Time(uint16_t t) {
	
}
