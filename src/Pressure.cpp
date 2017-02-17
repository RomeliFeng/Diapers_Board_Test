/*
 * Pressure.cpp
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#include "Pressure.h"
#include "U_ADC1.h"

#define PressureCh_1 ADC_Channel_3
#define PressureCh_2 ADC_Channel_4
#define PressureCh_3 ADC_Channel_5
#define PressureCh_4 ADC_Channel_6
#define PressureCh_5 ADC_Channel_7
#define PressureSampleTime ADC_SampleTime_239Cycles5

PressureClass Pressure;
WordtoByte_Typedef PressureData[5];

void PressureClass::RefreshData() {
	U_ADC1.RefreshData(PressureCh_1, PressureSampleTime);
	PressureData[0].word = U_ADC1Data;
	U_ADC1.RefreshData(PressureCh_2, PressureSampleTime);
	PressureData[1].word = U_ADC1Data;
	U_ADC1.RefreshData(PressureCh_3, PressureSampleTime);
	PressureData[2].word = U_ADC1Data;
	U_ADC1.RefreshData(PressureCh_4, PressureSampleTime);
	PressureData[3].word = U_ADC1Data;
	U_ADC1.RefreshData(PressureCh_5, PressureSampleTime);
	PressureData[4].word = U_ADC1Data;
}
