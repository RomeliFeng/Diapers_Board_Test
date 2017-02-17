/*
 * Flow.h
 *
 *  Created on: 2017Äê1ÔÂ8ÈÕ
 *      Author: Romeli
 */

#ifndef FLOW_H_
#define FLOW_H_

#include "cmsis_device.h"
#include "Typedef.h"

typedef enum _FlowCh_Typedef {
	FlowCh_1 = 0,
	FlowCh_2,
	FlowCh_3,
	FlowCh_4,
	FlowCh_5,
	FlowCh_6,
	FlowCh_7,
	FlowCh_8
} FlowCh_Typedef;

void FlowClear(FlowCh_Typedef ch);
void FlowUpdata(FlowCh_Typedef ch);

extern WordtoByte_Typedef FlowData[8];
#endif /* FLOW_H_ */
