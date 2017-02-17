/*
 * U_EXTI.h
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#ifndef U_EXTI_H_
#define U_EXTI_H_

#include "cmsis_device.h"

class U_EXTIClass {
public:
	U_EXTIClass() {
		GPIOInit();
		EXTIInit();
		NVICInit();
	}
private:
	void GPIOInit();
	void EXTIInit();
	void NVICInit();
};

#endif /* U_EXTI_H_ */
