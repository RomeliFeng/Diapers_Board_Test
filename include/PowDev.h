/*
 * PowDev.h
 *
 *  Created on: 2017Äê1ÔÂ3ÈÕ
 *      Author: Romeli
 */

#ifndef POWDEV_H_
#define POWDEV_H_

#include "cmsis_device.h"
#include "Typedef.h"

typedef enum _ValveCh_Typedef {
	ValveCh_1 = 0x01,
	ValveCh_2 = 0x02,
	ValveCh_3 = 0x04,
	ValveCh_4 = 0x08,
	ValveCh_5 = 0x10,
	ValveCh_6 = 0x20,
	ValveCh_7 = 0x40,
	ValveCh_8 = 0x80
} ValveCh_Typedef;

typedef enum _MotorCh_Typedef {
	MotorCh_1 = 0, MotorCh_2 = 2, MotorCh_3 = 4, MotorCh_4 = 6
} MotorCh_Typedef;

class PowDevClass {
public:
	void Valve(uint8_t status);
	void Valve(ValveCh_Typedef ch, FunctionalState NewState);
	void Motor(uint8_t status);
	void Motor(MotorCh_Typedef ch, uint8_t state);
};

extern PowDevClass PowDev;
extern volatile WordtoByte_Typedef PowDevStatus;

#endif
