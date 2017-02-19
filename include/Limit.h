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

typedef enum _LimitCh_Typedef {
	LimitCh_0,
	LimitCh_1,
	LimitCh_2,
	LimitCh_3,
	LimitCh_4,
	LimitCh_5,
	LimitCh_6,
	LimitCh_7
} LimitCh_Typedef;

void LimitUpdata(LimitCh_Typedef ch);

#endif /* LIMIT_H_ */
