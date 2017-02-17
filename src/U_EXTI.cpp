/*
 * U_EXTI.cpp
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#include "U_EXTI.h"
#include "Limit.h"
#include "Flow.h"

U_EXTIClass U_EXTI;

void U_EXTIClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void U_EXTIClass::EXTIInit() {
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2
			| EXTI_Line3 | EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7
			| EXTI_Line8 | EXTI_Line9 | EXTI_Line10 | EXTI_Line11 | EXTI_Line12
			| EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
}

void U_EXTIClass::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

extern "C" void EXTI_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line0)) {
		FlowUpdata(FlowCh_1);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	if (EXTI_GetITStatus(EXTI_Line1)) {
		FlowUpdata(FlowCh_2);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
	if (EXTI_GetITStatus(EXTI_Line2)) {
		FlowUpdata(FlowCh_3);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	if (EXTI_GetITStatus(EXTI_Line3)) {
		FlowUpdata(FlowCh_4);
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	if (EXTI_GetITStatus(EXTI_Line4)) {
		FlowUpdata(FlowCh_5);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	if (EXTI_GetITStatus(EXTI_Line5)) {
		FlowUpdata(FlowCh_6);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if (EXTI_GetITStatus(EXTI_Line6)) {
		FlowUpdata(FlowCh_7);
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if (EXTI_GetITStatus(EXTI_Line7)) {
		FlowUpdata(FlowCh_8);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if (EXTI_GetITStatus(EXTI_Line8)) {
		LimitUpdata(LimitCh_1);
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if (EXTI_GetITStatus(EXTI_Line9)) {
		LimitUpdata(LimitCh_2);
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if (EXTI_GetITStatus(EXTI_Line10)) {
		LimitUpdata(LimitCh_3);
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if (EXTI_GetITStatus(EXTI_Line11)) {
		LimitUpdata(LimitCh_4);
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if (EXTI_GetITStatus(EXTI_Line12)) {
		LimitUpdata(LimitCh_5);
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if (EXTI_GetITStatus(EXTI_Line13)) {
		LimitUpdata(LimitCh_6);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if (EXTI_GetITStatus(EXTI_Line14)) {
		LimitUpdata(LimitCh_7);
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if (EXTI_GetITStatus(EXTI_Line15)) {
		LimitUpdata(LimitCh_8);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

