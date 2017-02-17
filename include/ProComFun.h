/*
 * ProtocolComFunction.h
 *
 *  Created on: 2017Äê1ÔÂ11ÈÕ
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
#include "U_EXTI.h"
#include "Water.h"

void Check_Digital();
void Check_Analog(uint8_t* ProData);
void Check_Pressure();
void Check_Flow(uint8_t* ProData);
void Check_Limit();
void Check_Water();
void Check_Stepper(uint8_t* ProData);
void Contrl_Valve(uint8_t* ProData);
void Contrl_Motor(uint8_t* ProData);
void Contrl_Stepper(uint8_t* ProData);
void AutoContrl_Valve_With_Flow(uint8_t* ProData);
void AutoContrl_Motor_With_Time(uint8_t* ProData);
void AutoContrl_Motor_With_Limit(uint8_t* ProData);
void AutoContrl_Stepper_With_Limit(uint8_t* ProData);
void AutoContrl_Stepper_With_Presure(uint8_t* ProData);
void AutoContrl_Stepper_With_Step(uint8_t* ProData);
void AutoContrl_Stepper_With_Position(uint8_t* ProData);
void Setting_ClearFlow();
void Setting_SetStepperLimit(uint8_t* ProData);
void Setting_ClearStepperPosition(uint8_t* ProData);
void Special_Reset();
void Special_Stop();
void Special_Continue();
void Special_Online();
void Special_Cacel();
void Special_BootLoader();

bool AutoContrl_Stepper_With_Presure_Condition(uint8_t pre_selet, uint16_t pre);

extern ProtocolCom_Typedef Protocol_Command_Now;
extern uint8_t Protocol_DataLen_Now;
extern uint8_t Protocol_Data_Now[10];
extern uint8_t SendBuf[100];
extern uint8_t SendBufLen;

#endif /* PROTOCOLCOMFUNCTION_H_ */
