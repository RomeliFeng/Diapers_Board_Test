/*
 * Protect.cpp
 *
 *  Created on: 2017年2月20日
 *      Author: Romeli
 */

#include "Protect.h"

bool MotorMoveProtect(uint8_t status) { //判断电机移动操作是否在上限位操作
	if (LimitData.bit.bit0 != 0) {
		return true;
	} else {
		return false;
	}
}

bool StepperMoveProtect(StepperCh_Typedef ch) {
	switch (ch) {
	case StepperCh_1:
		switch (StepperDIR[StepperCh_1]) {
		case StepperDIR_Forward:
			if (LimitData.bit.bit0 == 2) {
				return false;
			}
			break;
		case StepperDIR_Backward:
			if (LimitData.bit.bit0 == 0) {
				return false;
			}
			break;
		default:
			break;
		}
		break;
	case StepperCh_2: //暂时未做保护

		break;
	default:
		break;
	}
	return true;
}
