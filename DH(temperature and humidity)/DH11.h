#ifndef DH11_H
#define DH11_H

#include "stm32f4xx_hal.h"

#define DH11_PORT GPIOD
#define DH11_PIN GPIO_PIN_11
#define TIMER TIM6

TIM_HandleTypeDef htim6;


void MX_GPIO_Init_In(void);
void MX_GPIO_Init_Out(void);
void MX_TIM_Init(void);
void getRealDataDH22(float*,uint8_t);
void getRealDataDH11(float*,uint8_t);

#endif
