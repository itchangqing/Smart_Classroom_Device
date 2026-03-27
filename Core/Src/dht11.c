#include "gpio.h"
#include "dht11.h"
#include "delay.h"

void DHT11_Init(void) {
	__HAL_RCC_GPIOC_CLK_ENABLE();
}



/*

@Brief 通过 DFT11 传感器采集一次温湿度数据

@Params
  temp 输出参数，接收温度数据，如果不需要可以传输 NULL
  humi 输出参数，接收湿度数据，如果不需要可以传输 NULL

@return
  采集成功返回 0，失败返回非零错误码，具体意义如下：
  1表示传感器损坏或连接异常
  2表示校验失败，通信可能收到干扰

@Author
  itchangqing

@Email
  *********

@Data
  2026-3-17 15:21
  
@Remark
本行是依赖微秒级延迟函数 Delay_US，移植时候需字节实现
  
@EXample

float t, h;

if(DHT11_Get(&t, &h) == 0) {
	//处理成功
}
else{
	//处理失败
}

*/

uint8_t DHT11_Get(float* temp, uint8_t* humi) {
	GPIO_InitTypeDef conf = {0};
	uint8_t data[5] = {0};
	
	conf.Pin = DHT11_PIN;
	conf.Speed = GPIO_SPEED_FREQ_LOW;		


	//把pc10设置成开漏输出
	conf.Mode = GPIO_MODE_AF_OD;
	HAL_GPIO_Init(DHT11_GPIO_PORT,&conf);

	HAL_GPIO_WritePin(DHT11_GPIO_PORT,DHT11_PIN,GPIO_PIN_RESET);
	HAL_Delay(20);

	//输入
	conf.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(DHT11_GPIO_PORT,&conf);
	Delay_US(13);
	Delay_US(40);

	//读到1就是不正常
	if(HAL_GPIO_ReadPin(DHT11_GPIO_PORT,DHT11_PIN)) {
		//连接异常
		return 1;
	}
	//此时就是正常的
	//等待响应低电平结束
	while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT,DHT11_PIN));

	Delay_US(120);

	for(uint8_t i = 0 ; i < 5 ; i++) {
		
		for(uint8_t j = 0; j < 8; j++) {
			//开始接受温度和湿度的40位数据
			//等待54那个结束
			while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT,DHT11_PIN));	

			Delay_US(50);//卡发1在中间

			data[i] <<= 1;//左移一位,高位移动

			if(HAL_GPIO_ReadPin(DHT11_GPIO_PORT,DHT11_PIN)) {
			//1
			Delay_US(50);//此时必定是下一个数据位的低电平，前半段

			data[i] += 1;
			}	  
			//0的时候不处理	
		}			
	}
	//校验失败
	if(data[0] + data[1] + data[2] +data[3] != data[4]) return 2;
	
	//校验成功
	if(humi != NULL) *humi = data[0];
	if(temp != NULL) *temp = data[2] + data[3] / 10.0;
	
	return 0;//成功返回0
	
}
