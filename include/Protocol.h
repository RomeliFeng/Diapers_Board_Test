/*
 * Protocol.h
 *
 *  Created on: 2017��1��7��
 *      Author: Romeli
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "cmsis_device.h"

typedef enum _PA_Typedef { //Protocol Analysis
	PA_Ok, PA_CommondError, PA_CheckSumError, PA_FrameError
} PA_Typedef;

typedef struct _PA_Struct_Typedef {
	PA_Typedef pa;
	uint8_t data[20];
	uint8_t code;
} PA_Struct_Typedef;

typedef enum _PC_Typedef { //Protocol Command
	PC_None,

	PC_Check_Digital = 0x00,
	PC_Check_Analog = 0x01,
	PC_Check_Pressure = 0x02,
	PC_Check_Flow = 0x03,
	PC_Check_Limit = 0x04,
	PC_Check_Water = 0x05,
	PC_Check_Stepper = 0x06,

	PC_Contrl_Valve = 0x20,
	PC_Contrl_Motor = 0x21,

	PC_AutoContrl_Valve_With_Flow = 0x40,
	PC_AutoContrl_Motor_With_Limit = 0x41,
	PC_AutoContrl_Motor_With_Time = 0x42,
	PC_AutoContrl_Stepper_With_Limit = 0x43,
	PC_AutoContrl_Stepper_With_Presure = 0x44,
	PC_AutoContrl_Stepper_With_Step = 0x45,
	PC_AutoContrl_Stepper_With_Position = 0x46,

	PC_Setting_ClearFlow = 0x60,
	PC_Setting_SetStepperLimit = 0x61,
	PC_Setting_ClearStepperPositon = 0x62,

	PC_Post_Get = 0xc0,
	PC_Post_Complete = 0xc1,
	PC_Post_CommandError = 0xc2,
	PC_Post_CheckSumError = 0xc3,
	PC_Post_Init = 0xc4,
	PC_Post_Busy = 0xc5,
	PC_Post_FrameError = 0xc6,
	PC_Post_BootloaderInit = 0xc7,
	PC_Post_SafeProtect = 0xc8,

	PC_Special_Reset = 0xe0,
	PC_Special_Stop = 0xe1,
	PC_Special_Continue = 0xe2,
	PC_Special_Cacel = 0xe3,
	PC_Special_Online = 0xe4,
	PC_Special_BootLoader = 0xe5
} PC_Typedef;

typedef enum _PSPC_Typedef { //Protocol Safe Protect Code
	PSPC_Motor
} PSPC_Typedef;

void Protocol_Analysis(PA_Struct_Typedef *pa_struct);
uint8_t Protocol_Format(PC_Typedef com, uint8_t datalen, uint8_t com_get,
		uint8_t* data, uint8_t* sendbuf);

extern PC_Typedef P_Receive_Command;
extern uint8_t P_Receive_Data[20];
extern uint8_t P_Receive_DataLen;

extern volatile bool P_Receive_Flag;
extern volatile bool P_Running_Flag;

#endif /* PROTOCOL_H_ */
