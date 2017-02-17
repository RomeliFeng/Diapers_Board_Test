/*
 * Limit.h
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#ifndef LIMIT_H_
#define LIMIT_H_

#include "cmsis_device.h"

extern uint8_t LimitData;

typedef enum _LimitCh_Typedef {
	LimitCh_1 = 0x01,
	LimitCh_2 = 0x02,
	LimitCh_3 = 0x04,
	LimitCh_4 = 0x08,
	LimitCh_5 = 0x10,
	LimitCh_6 = 0x20,
	LimitCh_7 = 0x40,
	LimitCh_8 = 0x80
} LimitCh_Typedef;

void LimitUpdata(LimitCh_Typedef ch);

#endif /* LIMIT_H_ */
