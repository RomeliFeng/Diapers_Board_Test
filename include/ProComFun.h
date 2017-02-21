/*
 * ProtocolComFunction.h
 *
 *  Created on: 2017��1��11��
 *      Author: Romeli
 */

#ifndef PROTOCOLCOMFUNCTION_H_
#define PROTOCOLCOMFUNCTION_H_

#include "cmsis_device.h"

#include "Delay.h"

#include "Analog.h"
#include "Digital.h"
#include "Flow.h"
#include "Limit.h"
#include "PowDev.h"
#include "Pressure.h"
#include "Protocol.h"
#include "Stepper.h"
#include "Water.h"

void PC_Check(P_Buf_Typedef *p_buf);
void PC_Contrl(P_Buf_Typedef *p_buf);
void PC_AutoContrl(P_Buf_Typedef *p_buf);
void PC_Setting(P_Buf_Typedef *p_buf);
void PC_Special(P_Buf_Typedef *p_buf);

void Check_Digital();
void Check_Analog(P_Buf_Typedef *p_buf);
void Check_Pressure();
void Check_Flow(P_Buf_Typedef *p_buf);
void Check_Limit();
void Check_Water();
void Check_Stepper(P_Buf_Typedef *p_buf);
void Contrl_Valve(P_Buf_Typedef *p_buf);
void Contrl_Motor(P_Buf_Typedef *p_buf);
void Contrl_Stepper(P_Buf_Typedef *p_buf);
void AutoContrl_Valve_With_Flow(P_Buf_Typedef *p_buf);
void AutoContrl_Motor_With_Time(P_Buf_Typedef *p_buf);
void AutoContrl_Motor_With_Limit(P_Buf_Typedef *p_buf);
void AutoContrl_Stepper_With_Limit(P_Buf_Typedef *p_buf);
void AutoContrl_Stepper_With_Presure(P_Buf_Typedef *p_buf);
void AutoContrl_Stepper_With_Step(P_Buf_Typedef *p_buf);
void AutoContrl_Stepper_With_Position(P_Buf_Typedef *p_buf);
void Setting_ClearFlow();
void Setting_SetStepperLimit(P_Buf_Typedef *p_buf);
void Setting_ClearStepperPosition(P_Buf_Typedef *p_buf);
void Special_Reset();
void Special_Stop();
void Special_Continue();
void Special_Online();
void Special_Cacel();
void Special_BootLoader();

bool AutoContrl_Stepper_With_Presure_Condition(uint8_t pre_selet, uint16_t pre);

extern PC_Typedef PC_Run;
extern uint8_t PC_Run_DataLen;
extern uint8_t PC_Run_Data[10];

#endif /* PROTOCOLCOMFUNCTION_H_ */
