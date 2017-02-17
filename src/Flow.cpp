/*
 * Flow.cpp
 *
 *  Created on: 2017Äê1ÔÂ8ÈÕ
 *      Author: Romeli
 */

#include "Flow.h"

#define FLOW1_PIN GPIO_Pin_0
#define FLOW2_PIN GPIO_Pin_1
#define FLOW3_PIN GPIO_Pin_2
#define FLOW4_PIN GPIO_Pin_3
#define FLOW5_PIN GPIO_Pin_4
#define FLOW6_PIN GPIO_Pin_5
#define FLOW7_PIN GPIO_Pin_6
#define FLOW8_PIN GPIO_Pin_7

#define Flow1_Read GPIO_ReadInputDataBit(GPIOD, FLOW1_PIN)
#define Flow2_Read GPIO_ReadInputDataBit(GPIOD, FLOW2_PIN)
#define Flow3_Read GPIO_ReadInputDataBit(GPIOD, FLOW3_PIN)
#define Flow4_Read GPIO_ReadInputDataBit(GPIOD, FLOW4_PIN)
#define Flow5_Read GPIO_ReadInputDataBit(GPIOD, FLOW5_PIN)
#define Flow6_Read GPIO_ReadInputDataBit(GPIOD, FLOW6_PIN)
#define Flow7_Read GPIO_ReadInputDataBit(GPIOD, FLOW7_PIN)
#define Flow8_Read GPIO_ReadInputDataBit(GPIOD, FLOW8_PIN)

WordtoByte_Typedef FlowData[8];

void FlowClear(FlowCh_Typedef ch) {
	switch (ch) {
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
	case FlowCh_8:
		FlowData[FlowCh_8].word = 0;
		break;
	default:
		break;
	}
}

void FlowUpdata(FlowCh_Typedef ch) {
	switch (ch) {
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
	case FlowCh_8:
		++FlowData[FlowCh_8].word;
		break;
	default:
		break;
	}
}

