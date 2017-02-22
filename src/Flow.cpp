/*
 * Flow.cpp
 *
 *  Created on: 2017��1��8��
 *      Author: Romeli
 */

#include "Flow.h"

#define FLOW0_PIN GPIO_Pin_0
#define FLOW1_PIN GPIO_Pin_1
#define FLOW2_PIN GPIO_Pin_2
#define FLOW3_PIN GPIO_Pin_3
#define FLOW4_PIN GPIO_Pin_4
#define FLOW5_PIN GPIO_Pin_5
#define FLOW6_PIN GPIO_Pin_6
#define FLOW7_PIN GPIO_Pin_7

WordtoByte_Typedef FlowData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

FlowClass Flow;

void FlowUpdate(FlowCh_Typedef ch) {
	FlowData[ch].word++;
}

void FlowClass::Clear(FlowCh_Typedef ch) {
	FlowData[ch].word = 0;
}

void FlowClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = FLOW0_PIN | FLOW1_PIN | FLOW2_PIN | FLOW3_PIN
			| FLOW4_PIN | FLOW5_PIN | FLOW6_PIN | FLOW7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void FlowClass::EXTIInit() {
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource7);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2
			| EXTI_Line3 | EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
}

void FlowClass::NVICInit() {
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
}

extern "C" void EXTI_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line0)) {
		FlowUpdate(FlowCh_0);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	if (EXTI_GetITStatus(EXTI_Line1)) {
		FlowUpdate(FlowCh_1);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
	if (EXTI_GetITStatus(EXTI_Line2)) {
		FlowUpdate(FlowCh_2);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	if (EXTI_GetITStatus(EXTI_Line3)) {
		FlowUpdate(FlowCh_3);
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	if (EXTI_GetITStatus(EXTI_Line4)) {
		FlowUpdate(FlowCh_4);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	if (EXTI_GetITStatus(EXTI_Line5)) {
		FlowUpdate(FlowCh_5);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if (EXTI_GetITStatus(EXTI_Line6)) {
		FlowUpdate(FlowCh_6);
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if (EXTI_GetITStatus(EXTI_Line7)) {
		FlowUpdate(FlowCh_7);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

