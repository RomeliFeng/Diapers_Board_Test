/*
 * Pressure.cpp
 *
 *  Created on: 2017��1��7��
 *      Author: Romeli
 */

#include "Pressure.h"
#include "U_ADC1.h"

#define Filter 5

#define PressureCh_0 ADC_Channel_3
#define PressureCh_1 ADC_Channel_4
#define PressureCh_2 ADC_Channel_5
#define PressureCh_3 ADC_Channel_6
#define PressureCh_4 ADC_Channel_7
#define PressureSampleTime ADC_SampleTime_28Cycles5

#define PRE0_PIN GPIO_Pin_3
#define PRE1_PIN GPIO_Pin_4
#define PRE2_PIN GPIO_Pin_5
#define PRE3_PIN GPIO_Pin_6
#define PRE4_PIN GPIO_Pin_7

PressureClass Pressure;
WordtoByte_Typedef PressureData[5] = { 0, 0, 0, 0, 0 };

void PressureClass::RefreshData() {
	U_ADC1.RegularChannelConfig(PressureCh_0, PressureSampleTime);
	U_ADC1.RefreshData();
	PressureData[0].word = U_ADC1Data;
	for (uint8_t i = 0; i < Filter; ++i) {
		U_ADC1.RefreshData();
		PressureData[0].word =
				PressureData[0].word > U_ADC1Data ?
						U_ADC1Data : PressureData[0].word;
	}

	U_ADC1.RegularChannelConfig(PressureCh_1, PressureSampleTime);
	U_ADC1.RefreshData();
	PressureData[1].word = U_ADC1Data;
	for (uint8_t i = 0; i < Filter; ++i) {
		U_ADC1.RefreshData();
		PressureData[1].word =
				PressureData[1].word > U_ADC1Data ?
						U_ADC1Data : PressureData[1].word;
	}

	U_ADC1.RegularChannelConfig(PressureCh_2, PressureSampleTime);
	U_ADC1.RefreshData();
	PressureData[2].word = U_ADC1Data;
	for (uint8_t i = 0; i < Filter; ++i) {
		U_ADC1.RefreshData();
		PressureData[2].word =
				PressureData[2].word > U_ADC1Data ?
						U_ADC1Data : PressureData[2].word;
	}

	U_ADC1.RegularChannelConfig(PressureCh_3, PressureSampleTime);
	U_ADC1.RefreshData();
	PressureData[3].word = U_ADC1Data;
	for (uint8_t i = 0; i < Filter; ++i) {
		U_ADC1.RefreshData();
		PressureData[3].word =
				PressureData[3].word > U_ADC1Data ?
						U_ADC1Data : PressureData[3].word;
	}

	U_ADC1.RegularChannelConfig(PressureCh_4, PressureSampleTime);
	U_ADC1.RefreshData();
	PressureData[4].word = U_ADC1Data;
	for (uint8_t i = 0; i < Filter; ++i) {
		U_ADC1.RefreshData();
		PressureData[4].word =
				PressureData[4].word > U_ADC1Data ?
						U_ADC1Data : PressureData[4].word;
	}

}

void PressureClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PRE0_PIN | PRE1_PIN | PRE2_PIN | PRE3_PIN
			| PRE4_PIN
	;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
