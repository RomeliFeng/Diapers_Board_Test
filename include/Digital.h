/*
 * Digital.h
 *
 *  Created on: 2016Äê12ÔÂ28ÈÕ
 *      Author: Romeli
 */

#ifndef DIGITAL_H_
#define DIGITAL_H_

#include "cmsis_device.h"

class DigitalClass {
public:
	DigitalClass(){
		GPIOInit();
	}
	void SelectCh(uint8_t ch);
	void RefreshData();
private:
	void GPIOInit();
};

extern DigitalClass Digital;
extern uint8_t Digital_Data[63];

#endif /* DIGITAL_H_ */
