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

bool MotorMoveProtect();
bool StepperMoveProtect(StepperDIR_Typedef dir);

#endif /* PROTECT_H_ */
