#include "adc.h"
#include "maikong.h"



void Maikong_Init(void) {

	HAL_ADCEx_Calibration_Start(&hadc2);

}


uint32_t Volume_Get(void) 
{
	HAL_ADC_Start(&hadc2);

	HAL_ADC_PollForConversion(&hadc2,5);

	uint32_t val = HAL_ADC_GetValue(&hadc2);
	

	HAL_ADC_Stop(&hadc2);

	return val;
}