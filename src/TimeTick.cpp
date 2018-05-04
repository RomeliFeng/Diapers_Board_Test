/*
 * TimeTick.cpp
 *
 *  Created on: 2017��1��11��
 *      Author: Romeli
 */

#include "TimeTick.h"

#define TIM_Interval 10  //n*100uS

TimeTickClass TimeTick;

void TimeTickClass::TIMInit() {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;
	TIM_TimeBaseStructure.TIM_Period = TIM_Interval;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
}

void TimeTickClass::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

void __attribute__((weak)) TimeTickISR() {

}

extern "C" void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		if (TimeTick.ThreadStart) {
			TimeTickISR();
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

