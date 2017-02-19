/*
 * Water.cpp
 *
 *  Created on: 2016��12��24��
 *      Author: Romeli
 */

#include "Water.h"
#include "HC165.h"

WaterClass Water;

BytetoBit_Typedef WaterData;

HC165Class HC165_Water = HC165Class(GPIOB, GPIO_Pin_8, GPIO_Pin_7, GPIO_Pin_6,
GPIO_Pin_5);

void WaterClass::RefreshData() {
	WaterData.byte = HC165_Water.Read(8);
}
