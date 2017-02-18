/*
 * Analog.h
 *
 *  Created on: 2016Äê12ÔÂ28ÈÕ
 *      Author: Romeli
 */

#ifndef ANALOG_H_
#define ANALOG_H_

#include "cmsis_device.h"
#include "Typedef.h"

typedef enum _AnalogBd_Typedef {
	AnalogBd_1 = 0,
	AnalogBd_2,
	AnalogBd_3,
	AnalogBd_4,
	AnalogBd_5,
	AnalogBd_6,
	AnalogBd_7,
	AnalogBd_8,
	AnalogBd_9,
	AnalogBd_10,
	AnalogBd_11,
	AnalogBd_12,
	AnalogBd_13,
	AnalogBd_14,
	AnalogBd_15,
	AnalogBd_16,

	AnalogBd_17,
	AnalogBd_18,
	AnalogBd_19,
	AnalogBd_20,
	AnalogBd_21,
	AnalogBd_22,
	AnalogBd_23,
	AnalogBd_24,
	AnalogBd_25,
	AnalogBd_26,
	AnalogBd_27,
	AnalogBd_28,
	AnalogBd_29,
	AnalogBd_30,
	AnalogBd_31,
	AnalogBd_32,

	AnalogBd_33,
	AnalogBd_34,
	AnalogBd_35,
	AnalogBd_36,
	AnalogBd_37,
	AnalogBd_38,
	AnalogBd_39,
	AnalogBd_40,
	AnalogBd_41,
	AnalogBd_42,
	AnalogBd_43,
	AnalogBd_44,
	AnalogBd_45,
	AnalogBd_46,
	AnalogBd_47,
	AnalogBd_48,
} AnalogBd_Typedef;
typedef enum _AnalogCh_Typedef {
	AnalogCh_1 = 0,
	AnalogCh_2,
	AnalogCh_3,
	AnalogCh_4,
	AnalogCh_5,
	AnalogCh_6,
	AnalogCh_7,
	AnalogCh_8,
	AnalogCh_9,
	AnalogCh_10,
	AnalogCh_11,
	AnalogCh_12,
	AnalogCh_13,
	AnalogCh_14,
	AnalogCh_15,
	AnalogCh_16
} AnalogCh_Typedef;

class AnalogClass {
public:
	AnalogClass();
	void SelectBd(AnalogBd_Typedef bo);
	void SelectCh(AnalogCh_Typedef ch);
	void SelectCh(AnalogCh_Typedef ch, uint16_t cnt);
	void RefreshData(AnalogBd_Typedef bo);
	void SampleSync();
private:
	void GPIOInit();
};

extern AnalogClass Analog;
extern WordtoByte_Typedef AnalogData[14];

#endif /* ANALOG_H_ */
