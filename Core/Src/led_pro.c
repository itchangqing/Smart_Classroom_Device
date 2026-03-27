#include <math.h>
#include "led_pro.h"
#include "tim.h"

static uint8_t	LED_Pro_hao = 0;	//挡位 0 1 2 3
static uint8_t	LED_Pro_mode = 0;	//慢呼吸模式为1 中速模式为2 快闪模式为3
static uint8_t	LED_Pro_tick = 0;	//定时 1为定时1min  2 为10min  3 为取消定时


void LED_Pro_Init(void) {
		
		//启动pwm信号正常输出
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
}



//智能台灯的控制,通过全局变量不同去执行不同的函数
//一次中断只能进行一个模式，所以其他的模式必须是0
void Set_hao(uint8_t temp) {
	LED_Pro_hao = temp;
	LED_Pro_mode = 0;	
	LED_Pro_tick = 0;
}

void Set_Mode(uint8_t temp) { 

	LED_Pro_mode = temp;
	LED_Pro_hao = 0;
	LED_Pro_tick = 0;
}

void Set_Tick(uint8_t temp) {
	LED_Pro_tick = temp;
	LED_Pro_hao = 0;	
	LED_Pro_mode = 0;
}


//获取当前全局变量的值
void Get_Every(uint8_t* temp1,uint8_t* temp2,uint8_t* temp3) {
	*temp1 = LED_Pro_hao;
	*temp2 = LED_Pro_mode;
	*temp3 = LED_Pro_tick;
}

//智能台灯的控制
void LED_PRO_Open(uint8_t dangwei) {
	
	if(dangwei == 0) {//关闭
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);//动态写入ccr
	//HAL_TIM_Base_Stop(&htim3);
		
	}
	else if(dangwei == 1) {//一档
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,166);//动态写入ccr
	}
	else if(dangwei == 2) {//二档
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,250);//动态写入ccr
	}
	else {//三档
	
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,499);//动态写入ccr
	}
	
}


//呼吸灯
void LED_PRO_Mode(uint8_t mode) {
	
		float k  = 0;
	
		if(mode == 1) k = 6;
		else if(mode == 2) k = 2;
		else k = 0.7;
	
		float t =  HAL_GetTick() * 0.001; //转换成秒
		
		float duty = 0.5 * sin(2 * 3.14 * t / k) + 0.5; //随秒而变化
		
		uint16_t ccr = duty * 500;//得到此时的ccr
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,ccr);//动态写入ccr
}



//定时
void LED_PRO_Time(uint16_t t) {
	
	//1min
	if(t == 1) {
	
	
	}
	//5min
	else {
	
	
	}
}
