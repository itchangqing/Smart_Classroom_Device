#include "tim.h"
#include "delay.h"


//微妙级别延迟函数
void Delay_US(uint16_t t) {
	//清除cnt
	__HAL_TIM_SetCounter(&htim2, 0);
	
	//启动时钟
	HAL_TIM_Base_Start(&htim2);
	
	while(__HAL_TIM_GetCounter(&htim2) < t);
	
	//关闭时钟
	HAL_TIM_Base_Stop(&htim2);

}
