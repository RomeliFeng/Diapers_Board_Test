/*
 * Digital.cpp
 *
 *  Created on: 2016��12��28��
 *      Author: Romeli
 */

#include "Digital.h"
#include "Delay.h"
#include "HC165.h"

#define S0_PIN GPIO_Pin_12
#define S1_PIN GPIO_Pin_11
#define S2_PIN GPIO_Pin_10
#define S3_PIN GPIO_Pin_9
#define S4_PIN GPIO_Pin_8

HC165Class HC165_Digital = HC165Class(GPIOB, GPIO_Pin_14, GPIO_Pin_13,
GPIO_Pin_15, GPIO_Pin_12);
DigitalClass Digital;

uint8_t Digital_Data[63];

void DigitalClass::SelectCh(uint8_t ch) {
	if (ch & 0x01) {
		GPIO_SetBits(GPIOE, S0_PIN);
	} else {
		GPIO_ResetBits(GPIOE, S0_PIN);
	}
	if (ch & 0x02) {
		GPIO_SetBits(GPIOE, S1_PIN);
	} else {
		GPIO_ResetBits(GPIOE, S1_PIN);
	}
	if (ch & 0x04) {
		GPIO_SetBits(GPIOE, S2_PIN);
	} else {
		GPIO_ResetBits(GPIOE, S2_PIN);
	}
	if (ch & 0x08) {
		GPIO_SetBits(GPIOE, S3_PIN);
	} else {
		GPIO_ResetBits(GPIOE, S3_PIN);
	}
	if (ch & 0x10) {
		GPIO_SetBits(GPIOE, S4_PIN);
	} else {
		GPIO_ResetBits(GPIOE, S4_PIN);
	}
}

void DigitalClass::RefreshData() {
	uint32_t data;
	uint8_t DelayCount;
	for (uint8_t ch = 0; ch < 21; ++ch) {
		SelectCh(ch); //ѡ��ͨ��
		DelayCount = 200;
		while (DelayCount--)
			;
		data = HC165_Digital.Read(24); //
		Digital_Data[ch * 3] = (uint8_t) data;
		Digital_Data[ch * 3 + 1] = (uint8_t) (data >> 8);
		Digital_Data[ch * 3 + 2] = (uint8_t) (data >> 16);
	}
}

void DigitalClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = S0_PIN | S1_PIN | S2_PIN | S3_PIN | S4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
