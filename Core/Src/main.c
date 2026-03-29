/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "dht11.h"
#include "light.h"
#include "maikong.h"
#include "led_pro.h"
#include "eeprom.h"
#include "sht30.h"
#include "oled.h"
#include "fan.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEVICE_ID "ppppp"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//用于定时记时间
uint32_t time = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  LED_Pro_Init();
	Light_Init();
	Maikong_Init();
	OLED_Display_On();
	OLED_Init();
	FAN_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	time = HAL_GetTick();
	int tick_friend = 0;
	float temp = 0, humi = 0;  //温度
	uint8_t volume = 0; //湿度和声音强度
	uint32_t light = 0; //光照强度
	
	char upload_data[100];
	char data_oled[30]; 	//在屏幕上面显示
	uint8_t led_hao = 0,led_mode = 0,led_tick = 0;//用于接收判断台灯目前处于什么状态
	uint8_t data = 0;//默认0
	
	uint32_t t = HAL_GetTick();//获取当前时间，用于周期性采集传感器数据
	EEPROM_Read(0X66, &data);//获取存储器里面的值，看看之前灯是什么状态
	
	//先设置一下led_pro的状态
	if(data <=3 ) Set_hao(data);
	else if(data == 4 || data == 5) Set_Mode(1);
	else Set_Tick(data - 6);//减去5 之后只能是0 或者 1 或者 2 ，1是定时1分钟，2是定时5分钟
	
	//显示过度动画
	OLED_Clear();
	for(uint8_t  x = 8; x <= 112; x ++) {
		
			OLED_ShowHao(x,0);
			OLED_ShowChar(x - 8, 0, ' ', 16);
		
	}
	
	OLED_ShowTEmp(0,1);
	OLED_ShowHUm(0,4);
	OLED_ShowTIttle(80,1);
	
//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); 
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); 
//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 499); 
	
  while (1)
  {		
	  //2s一次采集周期
	  if(HAL_GetTick() - t > 2000) {
		  t = HAL_GetTick();
//		// 使用DHT11采集一次温湿度数据
//		if(DHT11_Get(&temp, &humi) == 0)
//		{
//			// 如果采集温湿度数据成功，就上传云端
//			sprintf(upload_data, "%s/sensor/dht11 %.1f_%u\n", DEVICE_ID, temp, humi);
//			HAL_UART_Transmit(&huart2, (uint8_t*)upload_data, strlen(upload_data), 1000);
//		}
		  
		//使用SHT30采集温湿度
		if(SHT30_Get(&temp,&humi) == 0) {
			
			sprintf(upload_data,"%s/sensor/sht30 %.2f_%.2f\n",DEVICE_ID,temp, humi);
			HAL_UART_Transmit(&huart2,(uint8_t*)upload_data,strlen(upload_data),1000);
			
			sprintf(data_oled,"%c%.2f",':',temp);
			OLED_ShowString(32,1,(u8 *)data_oled,16);
			
			sprintf(data_oled,":%.2f%%RH",humi);
			OLED_ShowString(32,4,(u8 *)data_oled,16);
		
		}

		// 采集光照强度，并上传到云端
		light = Light_Get();
		sprintf(upload_data, "%s/sensor/light %u\n", DEVICE_ID, light);
		HAL_UART_Transmit(&huart2, (uint8_t*)upload_data, strlen(upload_data), 1000);

		//采集声音强度，上传云端
		volume = Volume_Get();
		sprintf(upload_data, "%s/sensor/volume %u\n", DEVICE_ID, volume);
		HAL_UART_Transmit(&huart2, (uint8_t*)upload_data, strlen(upload_data), 1000);
		// 其他传感器可以在这里扩展
		// ......
	}	
	  //获取当前台灯的状态值，判断目前是什么状态，从而执行台灯相应的动作
		Get_Every(&led_hao,&led_mode,&led_tick);
		
		//如果不是呼吸，那么就执行开灯，避免开灯产生的ccr影响呼吸
		if(led_mode == 0) {
			
			LED_PRO_Open(led_hao);
		}
		//如果是呼吸，那就执行
		else  LED_PRO_Mode(led_mode);
	
		
		//如果开启定时，开启定时当前时间只能改变一次，frined = 1
		if(tick_friend == 0 && led_tick > 0)LED_PRO_Time(led_tick),tick_friend = 1;
		
		//如果开启定时又关闭了定时，那么需要提前把frined给置为0，要不然下一次不会有新的时间
		if(led_tick == 0) tick_friend = 0;
		
		//这里是时间到达执行的动作
		if(led_tick > 0 && HAL_GetTick() - time > led_tick * 1000 * 3) {
		
			Set_Tick(0);
				
			LED_PRO_Time(0); //这里关闭定时，在上面会帮助我们关闭所有的灯，因为已经把挡位和呼吸都置为0
			
			tick_friend = 0;
		}
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
