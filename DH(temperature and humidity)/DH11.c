/*
	* THIS REALIZATION USES TIMER FOR MICROSECONDS COUNT.
	* THIS FILE HAVE NOT TIMER INITIALIZATION
*/

#include "DH11.h"

int read(uint8_t* data){
	 MX_GPIO_Init_Out();
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
	 HAL_Delay(100);
	 __HAL_TIM_SET_COUNTER(&htim6,0);
	 HAL_GPIO_WritePin(DH11_PORT,DH11_PIN,GPIO_PIN_RESET);
	 while((__HAL_TIM_GET_COUNTER(&htim6))<18000){};
	 __HAL_TIM_SET_COUNTER(&htim6,0);
	 HAL_GPIO_WritePin(DH11_PORT,DH11_PIN,GPIO_PIN_SET);
	 while((__HAL_TIM_GET_COUNTER(&htim6))<39){};
	 MX_GPIO_Init_In();
	 __HAL_TIM_SET_COUNTER(&htim6,0);
	 while(HAL_GPIO_ReadPin(DH11_PORT,DH11_PIN)==GPIO_PIN_RESET){};
	 while(HAL_GPIO_ReadPin(DH11_PORT,DH11_PIN)==GPIO_PIN_SET){};
	 uint8_t t2 = __HAL_TIM_GET_COUNTER(&htim6);
	 if(t2>=160)return 1;
	 for(uint8_t i=0;i<5;i++){
		 for(uint8_t j=0;j<8;j++){
			 data[i]=data[i]<<1;
			 while(HAL_GPIO_ReadPin(DH11_PORT,DH11_PIN)==GPIO_PIN_RESET){};
			 __HAL_TIM_SET_COUNTER(&htim6,0);
			 while(HAL_GPIO_ReadPin(DH11_PORT,DH11_PIN)==GPIO_PIN_SET){};
			 uint16_t tmp = __HAL_TIM_GET_COUNTER(&htim6);
			 if(tmp>28){
				 data[i]+=1;
			 }
		 }
	 }
	 MX_GPIO_Init_Out();
	 return 0;
}

int check(uint8_t* data){
	uint8_t sum=0;
	for(int i=0;i<4;i++){
		sum+=data[i];
	}
	if(sum==data[4])return 0;
	return 1;
}

void EnableClock(void){
	if(DH11_PORT==GPIOA)__HAL_RCC_GPIOA_CLK_ENABLE();
	if(DH11_PORT==GPIOB)__HAL_RCC_GPIOB_CLK_ENABLE();
	if(DH11_PORT==GPIOC)__HAL_RCC_GPIOC_CLK_ENABLE();
	if(DH11_PORT==GPIOD)__HAL_RCC_GPIOD_CLK_ENABLE();
}

void MX_GPIO_Init_In(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  EnableClock();

  GPIO_InitStruct.Pin = DH11_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DH11_PORT, &GPIO_InitStruct);
}

void MX_GPIO_Init_Out(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  EnableClock();

  HAL_GPIO_WritePin(DH11_PORT, DH11_PIN, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = DH11_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DH11_PORT, &GPIO_InitStruct);
}

void MX_TIM_Init(void)
{
  TIM_HandleTypeDef htim6;

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIMER;
  htim6.Init.Prescaler = 15;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0xff;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim6);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

}

void getRealDataDH11(float* res,uint8_t data){
	res[0] = data[2];
	res[0] += (data[3] & 0x0f) * 0.1;
	if (data[2] & 0x80) f *= -1;
	res[1] = data[0] + data[1] * 0.1;
}
void getRealDataDH22(float* res,uint8_t data){
	res[0] = ((uint16_t)(data[2] & 0x7F)) << 8 | data[3];
	res[0] *= 0.1;
	if (data[2] & 0x80) res[0] *= -1;
	res[1] = data[0] + data[1] * 0.1;
}

