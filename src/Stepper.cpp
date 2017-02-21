/*
 * Stepper.cpp
 *
 *  Created on: 2016��12��25��
 *      Author: Romeli
 */

#include <Stepper.h>
#include "Delay.h"
#include "Limit.h"

#define STEPPERSPEED 100

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

StepperClass Stepper;

TwoWordtoByteSigned_Typedef StepperPosition[2];
StepperDIR_Typedef StepperDIR[2];
BytetoBit_Typedef StepperLimit[2];

uint16_t StepperSpeed = STEPPERSPEED;

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

void StepperClass::MoveWithPosition(StepperCh_Typedef ch, int32_t position) {
	StepperDIR_Typedef dir =
			position > StepperPosition[ch].twoword ?
					StepperDIR_Forward : StepperDIR_Backward;
	uint32_t step =
			position - StepperPosition[ch].twoword > 0 ?
					position - StepperPosition[ch].twoword :
					-(position - StepperPosition[ch].twoword);
	Stepper.SetDIR(ch, dir);
	while (step--) {
		MoveOneStep(ch);
	}
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

void StepperClass::MoveOneStep(StepperCh_Typedef ch) {
	if ((StepperLimit[ch].byte != 0)
			&& ((LimitData.byte & StepperLimit[ch].byte)
					== StepperLimit[ch].byte)) {
		return;
	}
	switch (ch) {
	case StepperCh_1:
		PUL1_RESET;
		Delay_us(StepperSpeed);
		PUL1_SET;
		Delay_us(StepperSpeed);
		if (StepperDIR[ch] == StepperDIR_Forward) {
			++StepperPosition[ch].twoword;
		} else {
			--StepperPosition[ch].twoword;
		}
		break;
	case StepperCh_2:
		PUL2_RESET;
		Delay_us(StepperSpeed);
		PUL2_SET;
		Delay_us(StepperSpeed);
		if (StepperDIR[ch] == StepperDIR_Forward) {
			++StepperPosition[ch].twoword;
		} else {
			--StepperPosition[ch].twoword;
		}
		break;
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
