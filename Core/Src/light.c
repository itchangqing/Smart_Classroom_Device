#include "adc.h"
#include "light.h"

void Light_Init(void) {
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	
}

uint32_t Light_Get(void)
{
	uint32_t val;
	
	HAL_ADC_Start(&hadc1);
	
	HAL_ADC_PollForConversion(&hadc1, 5);
	
	val = HAL_ADC_GetValue(&hadc1);
	
	HAL_ADC_Stop(&hadc1);
	
	return val;
}
