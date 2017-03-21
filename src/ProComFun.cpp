/*
 * ProtocolComFunction.cpp
 *
 *  Created on: 2017��1��11��
 *      Author: Romeli
 */

#include <ProComFun.h>
#include "Protocol.h"
#include "U_USART1.h"

#define AppFlagAdd 0x08001c00 //bootloader 8k

typedef void (*pFun)(void);
pFun BootFun;

static DataBuf_Typedef SendBuf;

void PC_Check(P_Buf_Typedef *p_buf) {
	switch (p_buf->pc) {
	case PC_Check_Digital:
		Check_Digital();
		break;
	case PC_Check_Analog:
		Check_Analog(p_buf);
		break;
	case PC_Check_Pressure:
		Check_Pressure();
		break;
	case PC_Check_Flow:
		Check_Flow(p_buf);
		break;
	case PC_Check_Limit:
		Check_Limit();
		break;
	case PC_Check_Water:
		Check_Water();
		break;
	case PC_Check_Stepper:
		Check_Stepper(p_buf);
		break;
	default:
		break;
	}
}

void PC_Contrl(P_Buf_Typedef *p_buf) {
	switch (p_buf->pc) {
	case PC_Contrl_Valve:
		Contrl_Valve(p_buf);
		break;
	case PC_Contrl_Motor:
		Contrl_Motor(p_buf);
		break;
	default:
		break;
	}
}

void PC_AutoContrl(P_Buf_Typedef *p_buf) {
	switch (p_buf->pc) {
	case PC_AutoContrl_Valve_With_Flow:
		AutoContrl_Valve_With_Flow(p_buf);
		break;
	case PC_AutoContrl_Motor_With_Limit:
		AutoContrl_Motor_With_Limit(p_buf);
		break;
	case PC_AutoContrl_Motor_With_Time:
		AutoContrl_Motor_With_Time(p_buf);
		break;
	case PC_AutoContrl_Stepper_With_Limit:
		AutoContrl_Stepper_With_Limit(p_buf);
		break;
	case PC_AutoContrl_Stepper_With_Presure:
		AutoContrl_Stepper_With_Presure(p_buf);
		break;
	case PC_AutoContrl_Stepper_With_Step:
		AutoContrl_Stepper_With_Step(p_buf);
		break;
	case PC_AutoContrl_Stepper_With_Position:
		AutoContrl_Stepper_With_Position(p_buf);
		break;
	default:
		break;
	}
}

void PC_Setting(P_Buf_Typedef *p_buf) {
	switch (p_buf->pc) {
	case PC_Setting_ClearFlow:
		Setting_ClearFlow();
		break;
	case PC_Setting_SetStepperLimit:
		Setting_SetStepperLimit(p_buf);
		break;
	case PC_Setting_ClearStepperPositon:
		Setting_ClearStepperPosition(p_buf);
		break;
	default:
		break;
	}
}

void PC_Special(P_Buf_Typedef *p_buf) {
	uint16_t i = 1000; //等待收到指令的回复指令发送完成
	while (Serial.checkBusy()) {
		if (i-- == 0) {
			break;
		}
	}
	switch (p_buf->pc) {
	case PC_Special_Reset:
		Special_Reset();
		break;
	case PC_Special_Stop:
		Special_Stop();
		break;
	case PC_Special_Continue:
		Special_Continue();
		break;
	case PC_Special_Online:
		Special_Online();
		break;
	case PC_Special_Cacel:
		Special_Cacel();
		break;
	case PC_Special_BootLoader:
		Special_BootLoader();
		break;
	default:
		break;
	}
}

void Check_Digital() {
	Digital.RefreshData();
	Protocol_Send(PC_Post_Complete, 63, PC_Check_Digital, Digital_Data,
			&SendBuf);
}

void Check_Analog(P_Buf_Typedef *p_buf) {
	Analog.RefreshData((AnalogBd_Typedef) p_buf->data[0]);
	Protocol_Send(PC_Post_Complete, 28, PC_Check_Analog, (uint8_t*) AnalogData,
			&SendBuf);
}

void Check_Pressure() {
	Pressure.RefreshData();
	Protocol_Send(PC_Post_Complete, 10, PC_Check_Pressure,
			(uint8_t*) PressureData, &SendBuf);
}

void Check_Flow(P_Buf_Typedef *p_buf) {
	Protocol_Send(PC_Post_Complete, 2, PC_Check_Flow,
			FlowData[(uint8_t) p_buf->data[0]].byte, &SendBuf);
}

void Check_Limit() {
	Limit.RefreshData();
	Protocol_Send(PC_Post_Complete, 1, PC_Check_Limit, &LimitData.byte,
			&SendBuf);
}

void Check_Water() {
	Water.RefreshData();
	Protocol_Send(PC_Post_Complete, 1, PC_Check_Water, &WaterData.byte,
			&SendBuf);
}

void Check_Stepper(P_Buf_Typedef *p_buf) {
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	Protocol_Send(PC_Post_Complete, 4, PC_Check_Stepper,
			StepperPosition[ch].byte, &SendBuf);
}

void Contrl_Valve(P_Buf_Typedef *p_buf) {
	PowDev.Valve(p_buf->data[0]);
	Protocol_Send(PC_Post_Complete, 1, PC_Contrl_Valve,
			(uint8_t*) &PowDevStatus.byte[0], &SendBuf);
}

void Contrl_Motor(P_Buf_Typedef *p_buf) {
	PowDev.Motor(p_buf->data[0]);
	Protocol_Send(PC_Post_Complete, 1, PC_Contrl_Motor,
			(uint8_t*) &PowDevStatus.byte[1], &SendBuf);
}

void AutoContrl_Valve_With_Flow(P_Buf_Typedef *p_buf) {
	FlowCh_Typedef ch;
	WordtoByte_Typedef flowlimit;
	ch = (FlowCh_Typedef) p_buf->data[1];
	flowlimit.byte[0] = p_buf->data[2];
	flowlimit.byte[1] = p_buf->data[3];
	if ((p_buf->data[0] & 0x01) != 0) {
		PowDev.Valve(ValveCh_1, ENABLE);
	}
	if ((p_buf->data[0] & 0x02) != 0) {
		PowDev.Valve(ValveCh_2, ENABLE);
	}
	if ((p_buf->data[0] & 0x04) != 0) {
		PowDev.Valve(ValveCh_3, ENABLE);
	}
	if ((p_buf->data[0] & 0x08) != 0) {
		PowDev.Valve(ValveCh_4, ENABLE);
	}
	if ((p_buf->data[0] & 0x10) != 0) {
		PowDev.Valve(ValveCh_5, ENABLE);
	}
	if ((p_buf->data[0] & 0x20) != 0) {
		PowDev.Valve(ValveCh_6, ENABLE);
	}
	if ((p_buf->data[0] & 0x40) != 0) {
		PowDev.Valve(ValveCh_7, ENABLE);
	}
	if ((p_buf->data[0] & 0x80) != 0) {
		PowDev.Valve(ValveCh_8, ENABLE);
	}

	uint32_t timelast = millis();
	while (millis() - timelast < Valve_With_Flow_TimeLimit) { //超时时间60S
		if (FlowData[ch].word >= flowlimit.word) {
			break;
		}
	}

	if ((p_buf->data[0] & 0x01) != 0) {
		PowDev.Valve(ValveCh_1, DISABLE);
	}
	if ((p_buf->data[0] & 0x02) != 0) {
		PowDev.Valve(ValveCh_2, DISABLE);
	}
	if ((p_buf->data[0] & 0x04) != 0) {
		PowDev.Valve(ValveCh_3, DISABLE);
	}
	if ((p_buf->data[0] & 0x08) != 0) {
		PowDev.Valve(ValveCh_4, DISABLE);
	}
	if ((p_buf->data[0] & 0x10) != 0) {
		PowDev.Valve(ValveCh_5, DISABLE);
	}
	if ((p_buf->data[0] & 0x20) != 0) {
		PowDev.Valve(ValveCh_6, DISABLE);
	}
	if ((p_buf->data[0] & 0x40) != 0) {
		PowDev.Valve(ValveCh_7, DISABLE);
	}
	if ((p_buf->data[0] & 0x80) != 0) {
		PowDev.Valve(ValveCh_8, DISABLE);
	}

	uint8_t data[3];
	data[0] = PowDevStatus.byte[1];
	data[1] = FlowData[ch].byte[0];
	data[2] = FlowData[ch].byte[1];
	Protocol_Send(PC_Post_Complete, 3, PC_AutoContrl_Valve_With_Flow, data,
			&SendBuf);
}

void AutoContrl_Motor_With_Limit(P_Buf_Typedef *p_buf) {
	if ((p_buf->data[0] & (0x03 << 0)) != 0) {
		PowDev.Motor(MotorCh_1, (p_buf->data[0] >> MotorCh_1) & 0x03);
	}
	if ((p_buf->data[0] & (0x03 << 2)) != 0) {
		PowDev.Motor(MotorCh_2, (p_buf->data[0] >> MotorCh_2) & 0x03);
	}
	if ((p_buf->data[0] & (0x03 << 4)) != 0) {
		PowDev.Motor(MotorCh_3, (p_buf->data[0] >> MotorCh_3) & 0x03);
	}
	if ((p_buf->data[0] & (0x03 << 6)) != 0) {
		PowDev.Motor(MotorCh_4, (p_buf->data[0] >> MotorCh_4) & 0x03);
	}

	uint32_t timelast = millis();
	uint32_t count = 0;
	while (millis() - timelast < Motor_With_Limit_TimeLimit) { //30S
		Limit.RefreshData();
		if ((LimitData.byte & p_buf->data[1]) == p_buf->data[1]) {
			count++;
			if (count >= 10)
				break;
		}
	}

	if ((p_buf->data[0] & (0x03 << 0)) != 0) {
		PowDev.Motor(MotorCh_1, 0);
	}
	if ((p_buf->data[0] & (0x03 << 2)) != 0) {
		PowDev.Motor(MotorCh_2, 0);
	}
	if ((p_buf->data[0] & (0x03 << 4)) != 0) {
		PowDev.Motor(MotorCh_3, 0);
	}
	if ((p_buf->data[0] & (0x03 << 6)) != 0) {
		PowDev.Motor(MotorCh_4, 0);
	}

	uint8_t data[2];
	data[0] = PowDevStatus.byte[1];
	data[1] = LimitData.byte;

	Protocol_Send(PC_Post_Complete, 2, PC_AutoContrl_Motor_With_Limit, data,
			&SendBuf);
}

void AutoContrl_Motor_With_Time(P_Buf_Typedef *p_buf) {
	if ((p_buf->data[0] & (0x03 << 0)) != 0) {
		PowDev.Motor(MotorCh_1, (p_buf->data[0] >> MotorCh_1) & 0x03);
	}
	if ((p_buf->data[0] & (0x03 << 2)) != 0) {
		PowDev.Motor(MotorCh_2, (p_buf->data[0] >> MotorCh_2) & 0x03);
	}
	if ((p_buf->data[0] & (0x03 << 4)) != 0) {
		PowDev.Motor(MotorCh_3, (p_buf->data[0] >> MotorCh_3) & 0x03);
	}
	if ((p_buf->data[0] & (0x03 << 6)) != 0) {
		PowDev.Motor(MotorCh_4, (p_buf->data[0] >> MotorCh_4) & 0x03);
	}

	WordtoByte_Typedef timelimit;
	timelimit.byte[0] = p_buf->data[1];
	timelimit.byte[1] = p_buf->data[2];
	uint32_t timelast = millis();
	while (millis() - timelast < timelimit.word)
		;

	if ((p_buf->data[0] & (0x03 << 0)) != 0) {
		PowDev.Motor(MotorCh_1, 0);
	}
	if ((p_buf->data[0] & (0x03 << 2)) != 0) {
		PowDev.Motor(MotorCh_2, 0);
	}
	if ((p_buf->data[0] & (0x03 << 4)) != 0) {
		PowDev.Motor(MotorCh_3, 0);
	}
	if ((p_buf->data[0] & (0x03 << 6)) != 0) {
		PowDev.Motor(MotorCh_4, 0);
	}
	Protocol_Send(PC_Post_Complete, 1, PC_AutoContrl_Motor_With_Time,
			(uint8_t*) &PowDevStatus.byte[0], &SendBuf);
}

void AutoContrl_Stepper_With_Limit(P_Buf_Typedef *p_buf) {
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	StepperDIR_Typedef dir =
			((p_buf->data[0] & 0x40) == 0x40) ?
					StepperDIR_Backward : StepperDIR_Forward;
	Stepper.SetDIR(ch, dir);

	uint32_t timelast = millis();
	uint8_t count = 0;

	Stepper.AccCurve(ENABLE);
	while ((LimitData.byte & p_buf->data[1]) != p_buf->data[1]) {
		Limit.RefreshData();
		Stepper.MoveOneStep(ch);
		if (millis() - timelast > Stepper_With_Limit_TimeLimit) { //2min
			count++;
			if (count >= 10)
				break;
		}
	}

	Protocol_Send(PC_Post_Complete, 1, PC_AutoContrl_Stepper_With_Limit,
			&LimitData.byte, &SendBuf);
}

void AutoContrl_Stepper_With_Presure(P_Buf_Typedef *p_buf) {
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	StepperDIR_Typedef dir =
			((p_buf->data[0] & 0x40) == 0x40) ?
					StepperDIR_Backward : StepperDIR_Forward;
	Stepper.SetDIR(ch, dir);

	WordtoByte_Typedef pre;
	pre.byte[0] = p_buf->data[2];
	pre.byte[1] = p_buf->data[3];

	uint32_t timelast = millis();
	uint16_t pre_avg;
	uint8_t count = 0;

	Stepper.AccCurve(DISABLE);
	while (true) {
		Stepper.MoveOneStep(ch);
		pre_avg = AutoContrl_Stepper_With_Presure_Condition(p_buf->data[1]);
		if (dir == StepperDIR_Forward) {
			if (pre.word >= pre_avg) {
				count++;
				if (count >= 10)
					break;
			}
		} else {
			if (pre.word <= pre_avg) {
				count++;
				if (count >= 10)
					break;
			}
		}
		if (millis() - timelast > Stepper_With_Presure_TimeLimit) {
			break;
		}
	}

	Protocol_Send(PC_Post_Complete, 10, PC_AutoContrl_Stepper_With_Presure,
			(uint8_t*) PressureData, &SendBuf);
}

uint16_t AutoContrl_Stepper_With_Presure_Condition(uint8_t pre_selet) {
	Pressure.RefreshData();
	uint16_t preavg = 0;
	uint8_t prenum = 0;
	if ((pre_selet & 0x01) == 0x01) {
		prenum++;
		preavg += PressureData[0].word;
	}
	if ((pre_selet & 0x02) == 0x02) {
		prenum++;
		preavg += PressureData[1].word;
	}
	if ((pre_selet & 0x04) == 0x04) {
		prenum++;
		preavg += PressureData[2].word;
	}
	if ((pre_selet & 0x08) == 0x08) {
		prenum++;
		preavg += PressureData[3].word;
	}
	if ((pre_selet & 0x10) == 0x10) {
		prenum++;
		preavg += PressureData[4].word;
	}
	preavg /= prenum;
	return preavg;
}

void AutoContrl_Stepper_With_Step(P_Buf_Typedef *p_buf) {
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	StepperDIR_Typedef dir =
			((p_buf->data[0] & 0x40) == 0x40) ?
					StepperDIR_Backward : StepperDIR_Forward;
	Stepper.SetDIR(ch, dir);
	TwoWordtoByteSigned_Typedef step;
	step.byte[0] = p_buf->data[1];
	step.byte[1] = p_buf->data[2];
	step.byte[2] = p_buf->data[3];
	step.byte[3] = p_buf->data[4];
	uint8_t tmp;

	Stepper.AccCurve(ENABLE);
	Stepper.MoveWithStep(ch, step.twoword);
	Protocol_Send(PC_Post_Complete, 0, PC_AutoContrl_Stepper_With_Step, &tmp,
			&SendBuf);
}

void AutoContrl_Stepper_With_Position(P_Buf_Typedef *p_buf) {
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	uint32_t position = ((uint32_t) p_buf->data[1] << 24)

	| ((uint32_t) p_buf->data[2] << 16) | ((uint32_t) p_buf->data[3] << 8)
			| ((uint32_t) p_buf->data[4] << 0);

	Stepper.AccCurve(ENABLE);
	Stepper.MoveWithPosition(ch, position);
	Protocol_Send(PC_Post_Complete, 4, PC_AutoContrl_Stepper_With_Position,
			(uint8_t*) StepperPosition[ch].byte, &SendBuf);
}

void Setting_ClearFlow() {
	uint8_t tmp;
	for (uint8_t i = 0; i < 8; ++i) {
		FlowData[i].word = 0;
	}
	Protocol_Send(PC_Post_Complete, 0, PC_Setting_ClearFlow, &tmp, &SendBuf);
}

void Setting_SetStepperLimit(P_Buf_Typedef *p_buf) {
	uint8_t tmp;
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	StepperLimit[ch].byte = p_buf->data[1];
	Protocol_Send(PC_Post_Complete, 0, PC_Setting_SetStepperLimit, &tmp,
			&SendBuf);
}

void Setting_ClearStepperPosition(P_Buf_Typedef *p_buf) {
	uint8_t tmp;
	StepperCh_Typedef ch =
			((p_buf->data[0] & 0x80) == 0x80) ? StepperCh_2 : StepperCh_1;
	StepperPosition[ch].twoword = 0;
	Protocol_Send(PC_Post_Complete, 0, PC_Setting_ClearStepperPositon, &tmp,
			&SendBuf);
}

void Special_Reset() {
	NVIC_SystemReset();
}

void Special_Stop() {

}

void Special_Continue() {

}
void Special_Online() {
	Protocol_Send(PC_Post_Complete, 1, PC_Special_Online,
			(uint8_t *) &PC_LastAuto, &SendBuf);
}
void Special_Cacel() {

}

void Special_BootLoader() {
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_Unlock();
	FLASH_ProgramHalfWord(AppFlagAdd, 0xa5a5);
	FLASH_Lock();

	__disable_irq();
	vu32 StackAdd = *(vu32*) NVIC_VectTab_FLASH;
	if ((StackAdd & 0x2ffe0000) == 0x20000000) { //�ж�ջ����ַ�Ƿ���128k���ڣ�ջ��Ĭ������ߵ�ַ��
		BootFun = (pFun) (*(vu32*) (NVIC_VectTab_FLASH + 4)); //��λ��ַ
		__set_MSP(StackAdd);
		BootFun();
	}
}
