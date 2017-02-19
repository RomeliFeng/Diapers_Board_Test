/*
 * Flow.cpp
 *
 *  Created on: 2017��1��8��
 *      Author: Romeli
 */

#include "Flow.h"

#define FLOW0_PIN GPIO_Pin_0
#define FLOW1_PIN GPIO_Pin_1
#define FLOW2_PIN GPIO_Pin_2
#define FLOW3_PIN GPIO_Pin_3
#define FLOW4_PIN GPIO_Pin_4
#define FLOW5_PIN GPIO_Pin_5
#define FLOW6_PIN GPIO_Pin_6
#define FLOW7_PIN GPIO_Pin_7

WordtoByte_Typedef FlowData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void FlowClear(FlowCh_Typedef ch) {
	switch (ch) {
	case FlowCh_0:
		FlowData[FlowCh_0].word = 0;
		break;
	case FlowCh_1:
		FlowData[FlowCh_1].word = 0;
		break;
	case FlowCh_2:
		FlowData[FlowCh_2].word = 0;
		break;
	case FlowCh_3:
		FlowData[FlowCh_3].word = 0;
		break;
	case FlowCh_4:
		FlowData[FlowCh_4].word = 0;
		break;
	case FlowCh_5:
		FlowData[FlowCh_5].word = 0;
		break;
	case FlowCh_6:
		FlowData[FlowCh_6].word = 0;
		break;
	case FlowCh_7:
		FlowData[FlowCh_7].word = 0;
		break;

	default:
		break;
	}
}

void FlowUpdata(FlowCh_Typedef ch) {
	switch (ch) {
	case FlowCh_0:
		++FlowData[FlowCh_0].word;
		break;
	case FlowCh_1:
		++FlowData[FlowCh_1].word;
		break;
	case FlowCh_2:
		++FlowData[FlowCh_2].word;
		break;
	case FlowCh_3:
		++FlowData[FlowCh_3].word;
		break;
	case FlowCh_4:
		++FlowData[FlowCh_4].word;
		break;
	case FlowCh_5:
		++FlowData[FlowCh_5].word;
		break;
	case FlowCh_6:
		++FlowData[FlowCh_6].word;
		break;
	case FlowCh_7:
		++FlowData[FlowCh_7].word;
		break;
	default:
		break;
	}
}

