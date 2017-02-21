/*
 * PowDev.cpp
 *
 *  Created on: 2017��1��3��
 *      Author: Romeli
 */

#include "PowDev.h"
#include "HC595.h"
#include "Protect.h"

volatile WordtoByte_Typedef PowDevStatus = { 0 };

#define DS_PIN GPIO_Pin_6
#define OE_PIN GPIO_Pin_7
#define STCP_PIN GPIO_Pin_8
#define SHCP_PIN GPIO_Pin_9

HC595Class HC595_PowDev = HC595Class(GPIOC, DS_PIN, OE_PIN, STCP_PIN, SHCP_PIN);
PowDevClass PowDev;

bool PowDevClass::Valve(uint8_t status) {
	PowDevStatus.byte[0] = 0;
	PowDevStatus.byte[0] = status;
	HC595_PowDev.Write(PowDevStatus.word);
	return true;
}

bool PowDevClass::Valve(ValveCh_Typedef ch, FunctionalState NewState) {
	PowDevStatus.byte[0] &= ~ch; //�ر�Ҫ���õĵ�ŷ�
	if (NewState == ENABLE) {
		PowDevStatus.byte[0] |= ch;
	}
	HC595_PowDev.Write(PowDevStatus.word);
	return true;
}

bool PowDevClass::Motor(uint8_t status) {
	if (MotorMoveProtect()) {
		PowDevStatus.byte[1] = 0; //��ո߰�λ
		PowDevStatus.byte[1] = status;
		HC595_PowDev.Write(PowDevStatus.word);
	} else {
		return false;
	}
	return true;
}

bool PowDevClass::Motor(MotorCh_Typedef ch, uint8_t ctl) {
	if (MotorMoveProtect()) {
		PowDevStatus.byte[1] &= ~(0x03 << ch); //�������λ
		PowDevStatus.byte[1] |= ctl << ch;
		HC595_PowDev.Write(PowDevStatus.word);
	} else {
		return false;
	}
	return true;
}
