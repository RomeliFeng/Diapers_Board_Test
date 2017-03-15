/*
 * Protect.cpp
 *
 *  Created on: 2017年2月20日
 *      Author: Romeli
 */

#include "Protect.h"

bool MotorMoveProtect() { //判断电机移动操作是否在上限位操作
	if (LimitData.bit.bit0 != 0) {
		return true;
	} else {
		return false;
	}
}

bool StepperMoveProtect(StepperCh_Typedef ch) {
	Limit.RefreshData();
	switch (ch) {
	case StepperCh_1:
		switch (StepperDIR[ch]) {
		case StepperDIR_Forward:
			if ((LimitData.bit.bit0 == 1) || (!LimitData.bit.bit2 == 0)) {
				return false;
			}
			break;
		case StepperDIR_Backward:
			if (LimitData.bit.bit2 == 1) {
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
