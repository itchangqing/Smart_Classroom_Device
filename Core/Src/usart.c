/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "led.h"
#include "buzzer.h"
#include "fan.h"
#include "eeprom.h"
static uint8_t temp;
/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
	HAL_UART_Receive_IT(&huart2,&temp,sizeof(temp));
  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	if(huart == &huart2)  // 如果是串口2
	{
		if(temp == 'a')
		{
			// 打开 LED
			LED_Open();
		}
		else if(temp == 'b')
		{
			// 关闭 LED
			LED_Close();
		}
		else if(temp == 'c')
		{
			// 报警
			BUZZER_Open();
		}
		else if(temp == 'd')
		{
			// 静音
			BUZZER_Close();
		}
		else if(temp == 'e')
		{
			// 开风扇1
			FAN_Open_1();
		}
		else if(temp == '!')
		{
			// 开风扇2
			FAN_Open_2();
		}
		else if(temp == '@')
		{
			// 开风扇3
			FAN_Open_3();
		}
		else if(temp == '#')
		{
			// 翻转
			FAN_Toggle();
		}
		else if(temp == 'f')
		{
			// 关风扇
			FAN_Close();
		}
		else if(temp == 'g')
		{
			// 开启智能台灯 1 挡
			Set_hao(1);
			EEPROM_Write(0x66,1);
		}
		else if(temp == 'h')
		{
			// 开启智能台灯 2 挡
			Set_hao(2);
			EEPROM_Write(0x66,2);
		}
		else if(temp == 'i')
		{
			// 开启智能台灯 3 挡
			Set_hao(3);
			EEPROM_Write(0x66,3);
		}
		else if(temp == 'j')
		{
			// 关闭智能台灯
			Set_hao(0);
			EEPROM_Write(0x66,0);
		}
		else if(temp == 'k')
		{
			// 开启智能台灯慢慢呼吸
			Set_Mode(1);
			EEPROM_Write(0x66,4);
		}
		else if(temp == 'l')
		{
			// 开启智能台灯中速呼吸
			Set_Mode(2);
			EEPROM_Write(0x66,5);
		}
		else if(temp == 'm')
		{
			// 开启智能台灯快速呼吸
			Set_Mode(3);
			EEPROM_Write(0x66,5);
		}
		else if(temp == 'n')
		{
			// 开启智能台灯定时 1 分钟
			Set_Tick(1);
			EEPROM_Write(0x66,6);
		}
		else if(temp == 'o')
		{
			// 开启智能台灯定时 5 分钟
			Set_Tick(5);
			EEPROM_Write(0x66,7);
		}
		else if(temp == 'p')
		{
			// 关闭智能台灯定时
			Set_Tick(0);
			EEPROM_Write(0x66,8);
		}
		// 在这里扩展远程硬件控制功能
		// ......
		
		// 继续接收下一个字节数据
		HAL_UART_Receive_IT(&huart2,&temp,sizeof(temp));
	}
}
/* USER CODE END 1 */
