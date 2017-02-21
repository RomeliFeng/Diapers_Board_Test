/*
 * Limit.cpp
 *
 *  Created on: 2017��1��7��
 *      Author: Romeli
 */

#include "Limit.h"

#define LIM0_PIN GPIO_Pin_8
#define LIM1_PIN GPIO_Pin_9
#define LIM2_PIN GPIO_Pin_10
#define LIM3_PIN GPIO_Pin_11
#define LIM4_PIN GPIO_Pin_12
#define LIM5_PIN GPIO_Pin_13
#define LIM6_PIN GPIO_Pin_14
#define LIM7_PIN GPIO_Pin_15

BytetoBit_Typedef LimitData = { 0 };

LimitClass Limit;

void LimitClass::RefreshData() {
	LimitData.byte = ~(uint8_t) (GPIOD->IDR >> 8);
}

void LimitClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LIM0_PIN | LIM1_PIN | LIM2_PIN | LIM3_PIN
			| LIM4_PIN | LIM5_PIN | LIM6_PIN | LIM7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
