/*
 * Water.h
 *
 *  Created on: 2016��12��24��
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
extern BytetoBit_Typedef WaterData;

#endif /* WATER_H_ */
