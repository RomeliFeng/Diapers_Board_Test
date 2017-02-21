/*
 * Flow.h
 *
 *  Created on: 2017��1��8��
 *      Author: Romeli
 */

#ifndef FLOW_H_
#define FLOW_H_

#include "cmsis_device.h"
#include "Typedef.h"

typedef enum _FlowCh_Typedef {
	FlowCh_0 = 0,
	FlowCh_1,
	FlowCh_2,
	FlowCh_3,
	FlowCh_4,
	FlowCh_5,
	FlowCh_6,
	FlowCh_7
} FlowCh_Typedef;

class FlowClass {
public:
	FlowClass() {
		GPIOInit();
		EXTIInit();
		NVICInit();
	}
	void Clear(FlowCh_Typedef ch);
private:
	void GPIOInit();
	void EXTIInit();
	void NVICInit();
};

void FlowUpdate(FlowCh_Typedef ch);
extern WordtoByte_Typedef FlowData[8];
extern FlowClass Flow;

#endif /* FLOW_H_ */
