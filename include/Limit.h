/*
 * Limit.h
 *
 *  Created on: 2017��1��7��
 *      Author: Romeli
 */

#ifndef LIMIT_H_
#define LIMIT_H_

#include "cmsis_device.h"

extern BytetoBit_Typedef LimitData;

class LimitClass {
public:
	LimitClass() {
		GPIOInit();
	}
	void RefreshData();
private:
	void GPIOInit();
};

extern LimitClass Limit;

#endif /* LIMIT_H_ */
