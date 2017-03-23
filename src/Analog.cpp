/*
 * Analog.cpp
 *
 *  Created on: 2016��12��28��
 *      Author: Romeli
 */

#include "Analog.h"
#include "HC595.h"
#include "Delay.h"
#include "PWM.h"
#include "U_ADC1.h"

#define DS_PIN GPIO_Pin_0
#define OE_PIN GPIO_Pin_1
#define STCP_PIN GPIO_Pin_2
#define SHCP_PIN GPIO_Pin_3

#define SAM_Pin GPIO_Pin_0

#define AnalogBd1_16_Off 0x0c00
#define AnalogBd16_32_Off 0x0a00
#define AnalogBd32_48_Off 0x0600

#define AnalogCh_Size 14
#define AnalogCh_Exsit 13
#define AnalogCh_NTC 12

#define PWM_Period 18000
#define PWM_Duty 9000

//补偿参数应在修改修改采样时间后重新标定，偏移疑似信号源内阻导致
#define NTCCom 1.1414 //热敏电阻补偿
#define HRCom 1.414 //HR202补偿

volatile static uint16_t Status_Now = 0x0000;

HC595Class HC595_Analog = HC595Class(GPIOC, DS_PIN, OE_PIN, STCP_PIN, SHCP_PIN);
AnalogClass Analog;

WordtoByte_Typedef AnalogData[14];
volatile bool Analog_SamTrig = false;

const uint16_t AnalogBdAdd[48] = {
AnalogBd1_16_Off | (0 << 5),
AnalogBd1_16_Off | (1 << 5),
AnalogBd1_16_Off | (2 << 5),
AnalogBd1_16_Off | (3 << 5),
AnalogBd1_16_Off | (4 << 5),
AnalogBd1_16_Off | (5 << 5),
AnalogBd1_16_Off | (6 << 5),
AnalogBd1_16_Off | (7 << 5),
AnalogBd1_16_Off | (8 << 5),
AnalogBd1_16_Off | (9 << 5),
AnalogBd1_16_Off | (10 << 5),
AnalogBd1_16_Off | (11 << 5),
AnalogBd1_16_Off | (12 << 5),
AnalogBd1_16_Off | (13 << 5),
AnalogBd1_16_Off | (14 << 5),
AnalogBd1_16_Off | (15 << 5),

AnalogBd16_32_Off | (0 << 5),
AnalogBd16_32_Off | (1 << 5),
AnalogBd16_32_Off | (2 << 5),
AnalogBd16_32_Off | (3 << 5),
AnalogBd16_32_Off | (4 << 5),
AnalogBd16_32_Off | (5 << 5),
AnalogBd16_32_Off | (6 << 5),
AnalogBd16_32_Off | (7 << 5),
AnalogBd16_32_Off | (8 << 5),
AnalogBd16_32_Off | (9 << 5),
AnalogBd16_32_Off | (10 << 5),
AnalogBd16_32_Off | (11 << 5),
AnalogBd16_32_Off | (12 << 5),
AnalogBd16_32_Off | (13 << 5),
AnalogBd16_32_Off | (14 << 5),
AnalogBd16_32_Off | (15 << 5),

AnalogBd32_48_Off | (0 << 5),
AnalogBd32_48_Off | (1 << 5),
AnalogBd32_48_Off | (2 << 5),
AnalogBd32_48_Off | (3 << 5),
AnalogBd32_48_Off | (4 << 5),
AnalogBd32_48_Off | (5 << 5),
AnalogBd32_48_Off | (6 << 5),
AnalogBd32_48_Off | (7 << 5),
AnalogBd32_48_Off | (8 << 5),
AnalogBd32_48_Off | (9 << 5),
AnalogBd32_48_Off | (10 << 5),
AnalogBd32_48_Off | (11 << 5),
AnalogBd32_48_Off | (12 << 5),
AnalogBd32_48_Off | (13 << 5),
AnalogBd32_48_Off | (14 << 5),
AnalogBd32_48_Off | (15 << 5) };

const uint16_t AnalogChAdd[16] = { 0x0000, 0x0010, 0x0008, 0x0018, 0x0004,
		0x0014, 0x000c, 0x001c, 0x0002, 0x0012, 0x000a, 0x001a, 0x0006, 0x0016,
		0x000e, 0x001e };

/*初始化GPIO和采样所需的两路PWM*/
AnalogClass::AnalogClass() {
	GPIOInit();

	PWM.Init(PWM_Period, PWM_Period / 2);
//	PWM.SetPrescaler(3);
	PWM.SetPolarity(PWMCh_1, PWMPolarity_Low);
	PWM.SetPolarity(PWMCh_2, PWMPolarity_High);   //临时改动
	PWM.SetPolarity(PWMCh_3, PWMPolarity_Low);
	PWM.SetDuty(PWMCh_1, 4000);
}

void AnalogClass::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SAM_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*选中采样板卡*/
void AnalogClass::SelectBd(AnalogBd_Typedef bo) {
	Status_Now &= 0x001f;
	Status_Now |= AnalogBdAdd[bo];
	HC595_Analog.Write(Status_Now);
}

/*选中采样通道*/
void AnalogClass::SelectCh(AnalogCh_Typedef ch) {
	Status_Now &= ~(0x001f);
	Status_Now |= AnalogChAdd[ch];
	HC595_Analog.Write(Status_Now);

}

/*带延时的选择采样通道，基于TIM2定时器计算*/
void AnalogClass::SelectCh(AnalogCh_Typedef ch, uint16_t us) {
	uint16_t Count, usCount;
	//TIM周期=系统时钟/10000000/TIM分频数*延时微秒数
	usCount = SystemCoreClock / 1000000 / (TIM2->PSC + 1) * us;
	SelectCh(ch);
	//TIM2四分频 18000系统周期为1mS
	Count = TIM2->CNT;
	if (Count + usCount < TIM2->ARR) //判断是否会溢出
		while (TIM2->CNT - Count < usCount)//不会溢出
			;
	else {
		while ((TIM2->ARR - Count + TIM2->CNT) < usCount)//会溢出
			;
	}
}

void AnalogClass::RefreshData(AnalogBd_Typedef bo) {
	SelectBd(bo);
	SelectCh((AnalogCh_Typedef) AnalogCh_Exsit, 4);//先判断板卡是否存在
	U_ADC1.RefreshData(ADC_Channel_0, ADC_SampleTime_1Cycles5);
	if (U_ADC1Data > 100) { //板卡存在信号不存在
		for (int i = 0; i < AnalogCh_Size; ++i) {//填充默认数据
			AnalogData[i].word = 0xffff;
		}
		return;
	}
	AnalogData[AnalogCh_Exsit].word = 0; //板卡存在，赋值存在信号

	/*HR202信号采集*/
	uint8_t index = 0;
	uint8_t index_off;
	while (index < (AnalogCh_NTC)) { //在采集完成前循环
		index_off = index;
		SampleSync();
		while (index - index_off < 1) { //单个信号周期采集时间
			Analog.SelectCh((AnalogCh_Typedef) AnalogCh_Exsit, 4); //利用存在信号清除上次采样的漂移
			U_ADC1.RefreshData();
			Analog.SelectCh((AnalogCh_Typedef) index, 4); //切换采样通道 等待4us
			U_ADC1.RefreshData();
			AnalogData[index].word = U_ADC1Data * HRCom; //存储数据，进行信号补偿
			if (AnalogData[index].word > 4095)
				AnalogData[index].word = 4095;
			index++;
		}
	}
	/*热敏电阻值读取*/
	Analog.SelectCh((AnalogCh_Typedef) AnalogCh_Exsit, 4);
	U_ADC1.RefreshData();
	Analog.SelectCh((AnalogCh_Typedef) AnalogCh_NTC, 4);
	U_ADC1.RefreshData();
	AnalogData[AnalogCh_NTC].word = U_ADC1Data; //* NTCCom;
}

/*采样信号同步函数*/
void AnalogClass::SampleSync() {
	Analog_SamTrig = false;
	PWM.SwitchInterrupt(PWMCh_1, ENABLE);
	while (!Analog_SamTrig)
		;
}

/*CC1中断用于采样信号同步*/
void U_TIM2_CC1_ISR () {
	PWM.SwitchInterrupt(PWMCh_1, DISABLE);
	Analog_SamTrig = true;
}

