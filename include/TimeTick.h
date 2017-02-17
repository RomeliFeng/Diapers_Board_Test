/*
 * TimeTick.h
 *
 *  Created on: 2017Äê1ÔÂ11ÈÕ
 *      Author: Romeli
 */

#ifndef TIMETICK_H_
#define TIMETICK_H_

#include "cmsis_device.h"

class TimeTickClass {
public:
	TimeTickClass() {
		TIMInit();
		NVICInit();
	}
	void Enable();
	void Disable();
private:
	void TIMInit();
	void NVICInit();
};

extern void TimeTickISR();
extern TimeTickClass TimeTick;
#endif /* TIMETICK_H_ */
