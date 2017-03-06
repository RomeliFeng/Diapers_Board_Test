/*
 * Stepper.h
 *
 *  Created on: 2016��12��25��
 *      Author: Romeli
 *      PinMap: |PUL----PA12|
 *      		|DIR----PA11|
 *      		|EN-----PA8 |
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "stm32f10x.h"
#include "Typedef.h"

typedef enum _StepperDIR_Typedef {
	StepperDIR_Forward, StepperDIR_Backward
} StepperDIR_Typedef;

typedef enum _StepperCh_Typedef {
	StepperCh_1 = 0, StepperCh_2
} StepperCh_Typedef;

class StepperClass {
public:
	StepperClass() {
		GPIOInit();
		TIMInit();
		NVICInit();
		Lock();
	}
	void SetDIR(StepperCh_Typedef ch, StepperDIR_Typedef dir);
	void MoveWithStep(StepperCh_Typedef ch, uint32_t step);
	void MoveWithPosition(StepperCh_Typedef ch, int32_t position);
	void MoveOneStep(StepperCh_Typedef ch);
	void Lock();
	void Lock(StepperCh_Typedef ch);
	void Unlock(StepperCh_Typedef ch);
	void AccCurve(FunctionalState NewState);
private:
	void GPIOInit();
	void TIMInit();
	void NVICInit();
};

extern StepperClass Stepper;
extern TwoWordtoByteSigned_Typedef StepperPosition[2];
extern StepperDIR_Typedef StepperDIR[2];
extern BytetoBit_Typedef StepperLimit[2];

#endif /* MOTOR_H_ */
