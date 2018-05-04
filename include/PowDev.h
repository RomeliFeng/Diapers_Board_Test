/*
 * PowDev.h
 *
 *  Created on: 2017��1��3��
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

typedef enum _MotorContrl_Typedef {
	MotorContrl_Forward = 0b01,
	MotorContrl_Backward = 0b10,
	MotorContrl_Stop = 0b00
} MotorContrl_Typedef;

typedef struct _MotorChStatus_Typedef {
	uint8_t motor1 :2;
	uint8_t motor2 :2;
	uint8_t motor3 :2;
	uint8_t motor4 :2;
} MotorChStatus_Typedef;

typedef union _MotorStatus_Typedef {
	uint8_t all;
	MotorChStatus_Typedef motor;
} MotorStatus_Typedef;

#define PumpCh ValveCh_6

class PowDevClass {
public:
	static uint8_t Status[3];
	bool Valve(uint8_t status);
	bool Valve(ValveCh_Typedef ch, FunctionalState NewState);
	bool Motor(uint8_t status);
	bool Motor(MotorCh_Typedef ch, uint8_t ctl);
};

extern PowDevClass PowDev;
extern volatile WordtoByte_Typedef PowDevStatus;

#endif
