/*
 * Water.cpp
 *
 *  Created on: 2016Äê12ÔÂ24ÈÕ
 *      Author: Romeli
 */

#include "Water.h"
#include "HC165.h"

WaterClass Water;

uint8_t WaterData = 0;

HC165Class HC165_Water = HC165Class(GPIOB, GPIO_Pin_8, GPIO_Pin_7, GPIO_Pin_6,
GPIO_Pin_5);

void WaterClass::RefreshData() {
	WaterData = HC165_Water.Read(8);
}
