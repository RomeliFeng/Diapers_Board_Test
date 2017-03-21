/*
 * Stepper.cpp
 *
 *  Created on: 2016��12��25��
 *      Author: Romeli
 */

#include <Stepper.h>
#include "Delay.h"
#include "Limit.h"
#include "Protect.h"

#define EN1_SET GPIOE->BSRR = GPIO_Pin_4
#define EN1_RESET GPIOE->BRR = GPIO_Pin_4
#define PUL1_SET GPIOE->BSRR= GPIO_Pin_6
#define PUL1_RESET GPIOE->BRR = GPIO_Pin_6
#define DIR1_SET GPIOE->BSRR= GPIO_Pin_5
#define DIR1_RESET GPIOE->BRR = GPIO_Pin_5

#define EN2_SET GPIOE->BSRR = GPIO_Pin_0
#define EN2_RESET GPIOE->BRR = GPIO_Pin_0
#define PUL2_SET GPIOE->BSRR= GPIO_Pin_2
#define PUL2_RESET GPIOE->BRR = GPIO_Pin_2
#define DIR2_SET GPIOE->BSRR= GPIO_Pin_1
#define DIR2_RESET GPIOE->BRR = GPIO_Pin_1

#define SPEED_NORMAL 1500
#define SPEED_MAX 16000 //每秒最大步数
#define SPEED_ACC 50000 //最大加速度 每秒

StepperClass Stepper;

TwoWordtoByteSigned_Typedef StepperPosition[2];
StepperDIR_Typedef StepperDIR[2];
BytetoBit_Typedef StepperLimit[2];

uint16_t StepperSpeed = 0;

void StepperClass::SetDIR(StepperCh_Typedef ch, StepperDIR_Typedef dir) {
	StepperDIR[ch] = dir;
	switch (ch) {
	case StepperCh_1:
		switch (dir) {
		case StepperDIR_Forward:
			DIR1_RESET;
			break;
		case StepperDIR_Backward:
			DIR1_SET;
			break;
		default:
			break;
		}
		break;
	case StepperCh_2:
		switch (dir) {
		case StepperDIR_Forward:
			DIR2_RESET;
			break;
		case StepperDIR_Backward:
			DIR2_SET;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void StepperClass::MoveWithStep(StepperCh_Typedef ch, uint32_t step) {
	while (step--) {
		MoveOneStep(ch);
	}
}

void StepperClass::MoveOneStep(StepperCh_Typedef ch) {
	uint16_t StepperDelay;

	if ((StepperLimit[ch].byte != 0) //锁定的极限限位
	&& ((LimitData.byte & StepperLimit[ch].byte) == StepperLimit[ch].byte)) {
		return;
	}

	if (StepperMoveProtect(ch) == false) { //基于Protect.cpp的限位移动保护
		return;
	}

	StepperDelay = (1000000 / StepperSpeed) >> 1;

	switch (ch) {
	case StepperCh_1:
		PUL1_RESET;
		Delay_us(StepperDelay);
		PUL1_SET;
		Delay_us(StepperDelay);

		break;
	case StepperCh_2:
		PUL2_RESET;
		Delay_us(StepperDelay);
		PUL2_SET;
		Delay_us(StepperDelay);
		break;
	}
	if (StepperDIR[ch] == StepperDIR_Forward) {
		++StepperPosition[ch].twoword;
	} else {
		--StepperPosition[ch].twoword;
	}
}

void StepperClass::MoveWithPosition(StepperCh_Typedef ch, int32_t position) {
	StepperDIR_Typedef dir =
			position > StepperPosition[ch].twoword ?
					StepperDIR_Forward : StepperDIR_Backward;
	uint32_t step =
			position - StepperPosition[ch].twoword > 0 ?
					position - StepperPosition[ch].twoword :
					-(position - StepperPosition[ch].twoword);
	Stepper.SetDIR(ch, dir);

	MoveWithStep(ch, step);
}

void StepperClass::Lock() {
	EN1_SET;
	EN2_SET;
}

void StepperClass::Lock(StepperCh_Typedef ch) {
	switch (ch) {
	case StepperCh_1:
		EN1_SET;
		break;
	case StepperCh_2:
		EN2_SET;
		break;
	}
}

void StepperClass::Unlock(StepperCh_Typedef ch) {
	switch (ch) {
	case StepperCh_1:
		EN1_RESET;
		break;
	case StepperCh_2:
		EN2_RESET;
		break;
	}
}

void StepperClass::AccCurve(FunctionalState NewState) {
	if (NewState != DISABLE) {
		StepperSpeed = 1;
		TIM_Cmd(TIM4, ENABLE);
	} else {
		StepperSpeed = SPEED_NORMAL;
	}
}

void StepperClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void StepperClass::TIMInit() {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 180; //max acc 10000
	TIM_TimeBaseInitStructure.TIM_Period = 100000 / SPEED_ACC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_Cmd(TIM4, DISABLE);
}

void StepperClass::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

extern "C" void TIM4_IRQHandler() {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update)) {
		if (StepperSpeed < SPEED_MAX) {
			StepperSpeed++;
		} else {
			StepperSpeed = SPEED_MAX;
			TIM_Cmd(TIM4, DISABLE);
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
