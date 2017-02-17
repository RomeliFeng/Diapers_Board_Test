/*
 * Pressure.h
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#ifndef PRESSURE_H_
#define PRESSURE_H_

#include "cmsis_device.h"
#include "Typedef.h"

class PressureClass {
public:
	void RefreshData();
};

extern PressureClass Pressure;
extern WordtoByte_Typedef PressureData[5];

#endif /* PRESSURE_H_ */
