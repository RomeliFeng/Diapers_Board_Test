/*
 * Limit.cpp
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#include "Limit.h"

#define LIM1_PIN GPIO_Pin_8
#define LIM2_PIN GPIO_Pin_9
#define LIM3_PIN GPIO_Pin_10
#define LIM4_PIN GPIO_Pin_11
#define LIM5_PIN GPIO_Pin_12
#define LIM6_PIN GPIO_Pin_13
#define LIM7_PIN GPIO_Pin_14
#define LIM8_PIN GPIO_Pin_15

#define Lim1_Read GPIO_ReadInputDataBit(GPIOD, LIM1_PIN)
#define Lim2_Read GPIO_ReadInputDataBit(GPIOD, LIM2_PIN)
#define Lim3_Read GPIO_ReadInputDataBit(GPIOD, LIM3_PIN)
#define Lim4_Read GPIO_ReadInputDataBit(GPIOD, LIM4_PIN)
#define Lim5_Read GPIO_ReadInputDataBit(GPIOD, LIM5_PIN)
#define Lim6_Read GPIO_ReadInputDataBit(GPIOD, LIM6_PIN)
#define Lim7_Read GPIO_ReadInputDataBit(GPIOD, LIM7_PIN)
#define Lim8_Read GPIO_ReadInputDataBit(GPIOD, LIM8_PIN)

uint8_t LimitData = 0x00;

void LimitUpdata(LimitCh_Typedef ch) {
	switch (ch) {
	case LimitCh_1:
		if (Lim1_Read == 0) {
			LimitData |= LimitCh_1;
		} else {
			LimitData &= ~(LimitCh_1);
		}
		break;
	case LimitCh_2:
		if (Lim2_Read == 0) {
			LimitData |= LimitCh_2;
		} else {
			LimitData &= ~(LimitCh_2);
		}
		break;
	case LimitCh_3:
		if (Lim3_Read == 0) {
			LimitData |= LimitCh_3;
		} else {
			LimitData &= ~(LimitCh_3);
		}
		break;
	case LimitCh_4:
		if (Lim4_Read == 0) {
			LimitData |= LimitCh_4;
		} else {
			LimitData &= ~(LimitCh_4);
		}
		break;
	case LimitCh_5:
		if (Lim5_Read == 0) {
			LimitData |= LimitCh_5;
		} else {
			LimitData &= ~(LimitCh_5);
		}
		break;
	case LimitCh_6:
		if (Lim6_Read == 0) {
			LimitData |= LimitCh_6;
		} else {
			LimitData &= ~(LimitCh_6);
		}
		break;
	case LimitCh_7:
		if (Lim7_Read == 0) {
			LimitData |= LimitCh_7;
		} else {
			LimitData &= ~(LimitCh_7);
		}
		break;
	case LimitCh_8:
		if (Lim8_Read == 0) {
			LimitData |= LimitCh_8;
		} else {
			LimitData &= ~(LimitCh_8);
		}
		break;
	default:
		break;
	}
}
