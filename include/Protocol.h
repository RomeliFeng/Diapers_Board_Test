/*
 * Protocol.h
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: Romeli
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "cmsis_device.h"

typedef enum _ProtocolAn_Typedef {
	ProtocolAn_Ok,
	ProtocolAn_CommondError,
	ProtocolAn_CheckSumError,
	ProtocolAn_FrameError
} ProtocolAn_Typedef;

typedef struct {
	ProtocolAn_Typedef an;
	uint8_t code;
} ProtocolAnStruct_Typedef;

typedef enum _ProtocolCom_Typedef {
	ProtocolCom_None,

	ProtocolCom_Check_Digital = 0x00,
	ProtocolCom_Check_Analog = 0x01,
	ProtocolCom_Check_Pressure = 0x02,
	ProtocolCom_Check_Flow = 0x03,
	ProtocolCom_Check_Limit = 0x04,
	ProtocolCom_Check_Water = 0x05,
	ProtocolCom_Check_Stepper = 0x06,

	ProtocolCom_Contrl_Valve = 0x20,
	ProtocolCom_Contrl_Motor = 0x21,

	ProtocolCom_AutoContrl_Valve_With_Flow = 0x40,
	ProtocolCom_AutoContrl_Motor_With_Limit = 0x41,
	ProtocolCom_AutoContrl_Motor_With_Time = 0x42,
	ProtocolCom_AutoContrl_Stepper_With_Limit = 0x43,
	ProtocolCom_AutoContrl_Stepper_With_Presure = 0x44,
	ProtocolCom_AutoContrl_Stepper_With_Step = 0x45,
	ProtocolCom_AutoContrl_Stepper_With_Position = 0x46,

	ProtocolCom_Setting_ClearFlow = 0x60,
	ProtocolCom_Setting_SetStepperLimit = 0x61,
	ProtocolCom_Setting_ClearStepperPositon = 0x62,

	ProtocolCom_Post_Get = 0xc0,
	ProtocolCom_Post_Complete = 0xc1,
	ProtocolCom_Post_CommandError = 0xc2,
	ProtocolCom_Post_CheckSumError = 0xc3,
	ProtocolCom_Post_Init = 0xc4,
	ProtocolCom_Post_Busy = 0xc5,
	ProtocolCom_Post_FrameError = 0xc6,

	ProtocolCom_Special_Reset = 0xe0,
	ProtocolCom_Special_Stop = 0xe1,
	ProtocolCom_Special_Continue = 0xe2,
	ProtocolCom_Special_Cacel = 0xe3,
	ProtocolCom_Special_Online = 0xe4,
	ProtocolCom_Special_BootLoader = 0xe5
} ProtocolCom_Typedef;

void Protocol_Analysis(ProtocolAnStruct_Typedef* ProStruct);
void Protocol_Format(ProtocolCom_Typedef com, uint8_t datalen, uint8_t com_get,
		uint8_t* data, uint8_t* sendbuf, uint8_t* sendbuflen);

extern ProtocolCom_Typedef Protocol_Command;
extern volatile bool Protocol_Command_Flag;
extern volatile bool Protocol_Command_Running_Flag;
extern uint8_t Protocol_DataLen;
extern uint8_t Protocol_Data[10];

#endif /* PROTOCOL_H_ */
