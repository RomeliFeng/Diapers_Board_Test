/*
 * Water.h
 *
 *  Created on: 2016Äê12ÔÂ24ÈÕ
 *      Author: Romeli
 */

#ifndef WATER_H_
#define WATER_H_

#include "cmsis_device.h"

class WaterClass {
public:
	void RefreshData();
};

extern WaterClass Water;
extern uint8_t WaterData;

#endif /* WATER_H_ */
