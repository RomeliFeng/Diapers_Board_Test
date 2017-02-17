/*
 * PowDev.cpp
 *
 *  Created on: 2017年1月3日
 *      Author: Romeli
 */

#include "PowDev.h"
#include "HC595.h"

volatile WordtoByte_Typedef PowDevStatus = { 0x0000 };

#define DS_PIN GPIO_Pin_6
#define OE_PIN GPIO_Pin_7
#define STCP_PIN GPIO_Pin_8
#define SHCP_PIN GPIO_Pin_9

HC595Class HC595_PowDev = HC595Class(GPIOC, DS_PIN, OE_PIN, STCP_PIN, SHCP_PIN);
PowDevClass PowDev;

void PowDevClass::Valve(uint8_t status) {
	PowDevStatus.byte[0] = 0; //清空低八位，关闭所有电磁阀
	PowDevStatus.byte[0] = status; //调整为设置状态
	HC595_PowDev.Write(PowDevStatus.word);
}

void PowDevClass::Valve(ValveCh_Typedef ch, FunctionalState NewState) {
	PowDevStatus.byte[0] &= ~ch; //关闭要设置的电磁阀
	if (NewState == ENABLE) {
		PowDevStatus.byte[0] |= ch;
	}
	HC595_PowDev.Write(PowDevStatus.word);
}

void PowDevClass::Motor(uint8_t status) {
	PowDevStatus.byte[1] = 0; //清空高八位
	PowDevStatus.byte[1] = status;
	HC595_PowDev.Write(PowDevStatus.word);
}

void PowDevClass::Motor(MotorCh_Typedef ch, uint8_t state) {
	PowDevStatus.byte[1] &= ~(0x03 << ch); //清空设置位
	PowDevStatus.byte[1] |= state << ch;
	HC595_PowDev.Write(PowDevStatus.word);
}
