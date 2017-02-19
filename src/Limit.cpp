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

#define Lim0_Read (GPIOD->IDR & LIM0_PIN) == RESET ? 1 : 0
#define Lim1_Read (GPIOD->IDR & LIM1_PIN) == RESET ? 1 : 0
#define Lim2_Read (GPIOD->IDR & LIM2_PIN) == RESET ? 1 : 0
#define Lim3_Read (GPIOD->IDR & LIM3_PIN) == RESET ? 1 : 0
#define Lim4_Read (GPIOD->IDR & LIM4_PIN) == RESET ? 1 : 0
#define Lim5_Read (GPIOD->IDR & LIM5_PIN) == RESET ? 1 : 0
#define Lim6_Read (GPIOD->IDR & LIM6_PIN) == RESET ? 1 : 0
#define Lim7_Read (GPIOD->IDR & LIM7_PIN) == RESET ? 1 : 0

BytetoBit_Typedef LimitData = { 0 };

void LimitUpdata(LimitCh_Typedef ch) {
	switch (ch) {
	case LimitCh_0:
		LimitData.bit.bit0 = Lim0_Read;
		break;
	case LimitCh_1:
		LimitData.bit.bit1 = Lim1_Read;
		break;
	case LimitCh_2:
		LimitData.bit.bit2 = Lim2_Read;
		break;
	case LimitCh_3:
		LimitData.bit.bit3 = Lim3_Read;
		break;
	case LimitCh_4:
		LimitData.bit.bit4 = Lim4_Read;
		break;
	case LimitCh_5:
		LimitData.bit.bit5 = Lim5_Read;
		break;
	case LimitCh_6:
		LimitData.bit.bit6 = Lim6_Read;
		break;
	case LimitCh_7:
		LimitData.bit.bit7 = Lim7_Read;
		break;
	default:
		break;
	}
}
