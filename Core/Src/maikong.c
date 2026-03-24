#include "adc.h"
#include "maikong.h"

uint32_t Volume_Get(void) 
{
	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1,5);

	uint32_t val = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);

	return val;
}