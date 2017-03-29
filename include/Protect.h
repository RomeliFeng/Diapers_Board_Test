/*
 * Protect.h
 *
 *  Created on: 2017年2月20日
 *      Author: Romeli
 */

#ifndef PROTECT_H_
#define PROTECT_H_

#include "cmsis_device.h"

#include "PowDev.h"
#include "Limit.h"
#include "Stepper.h"

#define Valve_With_Flow_Total_TimeLimit 600000
#define Valve_With_Flow_Interval_TimeLimit 30000
//#define Motor_With_Time_TimeLimit 10000
#define Motor_With_Limit_TimeLimit 10000
#define Stepper_With_Limit_TimeLimit 120000
#define Stepper_With_Pressure_TimeLimit 120000
#define Stepper_With_Pressure_PressureLimit 10000/6
//#define Stepper_With_Step_TimeLimit 120000
//#define Stepper_With_Position_TimeLimit 120000

bool MotorMoveProtect();
bool StepperMoveProtect(StepperCh_Typedef ch);

#endif /* PROTECT_H_ */
