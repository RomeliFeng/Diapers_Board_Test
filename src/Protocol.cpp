/*
 * Protocol.cpp
 *
 *  Created on: 2017年1月7日
 *      Author: Romeli
 */

#include "Protocol.h"
#include "U_USART1.h"
#include "TimeTick.h"

#define COMMAND_OFFSET 0
#define DATALEN_OFFSET 1
#define DATA_OFFSET 2

ProtocolCom_Typedef Protocol_Command = ProtocolCom_None;
volatile bool Protocol_Command_Flag = false;
volatile bool Protocol_Command_Running_Flag = false;

uint8_t Protocol_DataLen = 0;
uint8_t Protocol_Data[10];
uint8_t Protocol_Sum = 0;
uint8_t Protocol_Sum_Check = 0;

uint8_t Protocol_SendBuf[100];

void Serial_Event() {
	ProtocolAnStruct_Typedef ProStruct;
	Protocol_Analysis(&ProStruct);
	uint8_t sendbuf[30];
	uint8_t command_get = Protocol_SendBuf[COMMAND_OFFSET];
	uint8_t buftmp[20];
	uint8_t bufindex = 0;
	switch (ProStruct.an) {
	case ProtocolAn_CommondError: //指令解析失败
		for (uint8_t i = 0; i < Protocol_SendBuf[DATALEN_OFFSET]; ++i) {
			buftmp[bufindex++] = Protocol_SendBuf[DATA_OFFSET + i];
		}
		Protocol_Format(ProtocolCom_Post_CommandError, bufindex, command_get,
				buftmp, sendbuf, &bufindex);
		Serial.print(sendbuf, bufindex);
		break;
	case ProtocolAn_CheckSumError: //指令校验失败
		uint8_t i;
		for (i = 0; i < Protocol_SendBuf[DATALEN_OFFSET]; ++i) {
			buftmp[bufindex++] = Protocol_SendBuf[DATA_OFFSET + i];
		}
		buftmp[bufindex++] = ProStruct.code; //返回的计算的校验值
		Protocol_Format(ProtocolCom_Post_CheckSumError, bufindex, command_get,
				buftmp, sendbuf, &bufindex);
		Serial.print(sendbuf, bufindex);
		break;
	case ProtocolAn_FrameError: //帧格式校验失败
		Protocol_Format(ProtocolCom_Post_CommandError, 1, command_get,
				&ProStruct.code, sendbuf, &bufindex);
		Serial.print(sendbuf, bufindex);
		break;
	default:
		break;
	}
}

void Protocol_Analysis(ProtocolAnStruct_Typedef* ProStruct) {
	if (!((Serial.read() == 0xff) && (Serial.read() == 0xff))) { //起始位不对
		(*ProStruct).an = ProtocolAn_FrameError;
		(*ProStruct).code = 0x00;
		return;
	}
	if (Serial.available() > 20) { //超长命令
		(*ProStruct).an = ProtocolAn_FrameError;
		(*ProStruct).code = 0x01;
		return;
	}
	uint8_t frameLen = Serial.available();
	Serial.read(Protocol_SendBuf, frameLen);
	if (frameLen != 1 + 1 + Protocol_SendBuf[DATALEN_OFFSET] + 1) { //数据帧长度不对
		(*ProStruct).an = ProtocolAn_FrameError;
		(*ProStruct).code = 0x02;
		return;
	}
	switch (Protocol_SendBuf[COMMAND_OFFSET]) {
	case ProtocolCom_Check_Digital:
	case ProtocolCom_Check_Analog:
	case ProtocolCom_Check_Pressure:
	case ProtocolCom_Check_Flow:
	case ProtocolCom_Check_Limit:
	case ProtocolCom_Check_Water:
	case ProtocolCom_Check_Stepper:
	case ProtocolCom_Contrl_Valve:
	case ProtocolCom_Contrl_Motor:
	case ProtocolCom_AutoContrl_Valve_With_Flow:
	case ProtocolCom_AutoContrl_Motor_With_Limit:
	case ProtocolCom_AutoContrl_Motor_With_Time:
	case ProtocolCom_AutoContrl_Stepper_With_Limit:
	case ProtocolCom_AutoContrl_Stepper_With_Presure:
	case ProtocolCom_AutoContrl_Stepper_With_Step:
	case ProtocolCom_AutoContrl_Stepper_With_Position:
	case ProtocolCom_Setting_ClearFlow:
	case ProtocolCom_Setting_SetStepperLimit:
	case ProtocolCom_Setting_ClearStepperPositon:
	case ProtocolCom_Special_Reset:
	case ProtocolCom_Special_Stop:
	case ProtocolCom_Special_Continue:
	case ProtocolCom_Special_Online:
	case ProtocolCom_Special_Cacel:
	case ProtocolCom_Special_BootLoader:
		Protocol_Command = (ProtocolCom_Typedef) Protocol_SendBuf[COMMAND_OFFSET];
		break;
	default: //未知命令
		(*ProStruct).an = ProtocolAn_CommondError;
		return;
	}

	Protocol_Sum = Protocol_SendBuf[DATA_OFFSET + Protocol_SendBuf[DATALEN_OFFSET]]; //获取sum
	Protocol_Sum_Check = 0; //计算前先清零
	Protocol_DataLen = Protocol_SendBuf[DATALEN_OFFSET]; //获取数据长度
	Protocol_Sum_Check = Protocol_Command + Protocol_DataLen; //计算sum
	for (uint8_t i = 0; i < Protocol_SendBuf[DATALEN_OFFSET]; ++i) {
		Protocol_Data[i] = Protocol_SendBuf[DATA_OFFSET + i]; //搬移数据并计算sum
		Protocol_Sum_Check += Protocol_SendBuf[DATA_OFFSET + i];
	}
	if (Protocol_Sum_Check != Protocol_Sum) { //计算的sum和收到的不符
		(*ProStruct).an = ProtocolAn_CheckSumError;
		(*ProStruct).code = Protocol_Sum_Check;
		return;
	}

	uint8_t sendbuf[100];
	uint8_t sendbuflen;
	Protocol_Format(ProtocolCom_Post_Get, Protocol_DataLen, Protocol_Command,
			(uint8_t*) Protocol_Data, sendbuf, &sendbuflen); //格式化指令收到回复
	Serial.print(sendbuf, sendbuflen);
	(*ProStruct).an = ProtocolAn_Ok;

	Protocol_Command_Flag = true; //数据验证成功
}

void Protocol_Format(ProtocolCom_Typedef com, uint8_t datalen, uint8_t com_get,
		uint8_t* data, uint8_t* sendbuf, uint8_t* sendbuflen) {
	uint8_t index = 0;
	uint8_t sum = 0;

	sendbuf[index++] = 0xff;
	sendbuf[index++] = 0xff; //起始位
	sendbuf[index++] = com; //指令
//以下数据开始计算求和
	sendbuf[index++] = datalen + 1;	//数据长度
	sum = com + datalen + 1;
	sendbuf[index++] = com_get;	//返回数据基于的指令
	sum += com_get;
	for (uint8_t i = 0; i < datalen; ++i) {	//填充数据
		sendbuf[index++] = *(data + i);
		sum += *(data + i);
	}
	sendbuf[index++] = sum;	//sum
	*sendbuflen = 2 + 1 + 1 + 1 + datalen + 1; //起始位+指令+指令长度+收到的指令+回复的数据+校验
}
