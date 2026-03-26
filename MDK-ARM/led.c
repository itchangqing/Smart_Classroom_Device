#include <math.h>
#include "led.h"
#include "gpio.h"
#include "tim.h"




void LED_Open(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
}


void LED_Close(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
}


