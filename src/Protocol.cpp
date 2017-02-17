/*
 * Protocol.cpp
 *
 *  Created on: 2017��1��7��
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
	case ProtocolAn_CommondError: //ָ�����ʧ��
		for (uint8_t i = 0; i < Protocol_SendBuf[DATALEN_OFFSET]; ++i) {
			buftmp[bufindex++] = Protocol_SendBuf[DATA_OFFSET + i];
		}
		Protocol_Format(ProtocolCom_Post_CommandError, bufindex, command_get,
				buftmp, sendbuf, &bufindex);
		Serial.print(sendbuf, bufindex);
		break;
	case ProtocolAn_CheckSumError: //ָ��У��ʧ��
		uint8_t i;
		for (i = 0; i < Protocol_SendBuf[DATALEN_OFFSET]; ++i) {
			buftmp[bufindex++] = Protocol_SendBuf[DATA_OFFSET + i];
		}
		buftmp[bufindex++] = ProStruct.code; //���صļ����У��ֵ
		Protocol_Format(ProtocolCom_Post_CheckSumError, bufindex, command_get,
				buftmp, sendbuf, &bufindex);
		Serial.print(sendbuf, bufindex);
		break;
	case ProtocolAn_FrameError: //֡��ʽУ��ʧ��
		Protocol_Format(ProtocolCom_Post_CommandError, 1, command_get,
				&ProStruct.code, sendbuf, &bufindex);
		Serial.print(sendbuf, bufindex);
		break;
	default:
		break;
	}
}

void Protocol_Analysis(ProtocolAnStruct_Typedef* ProStruct) {
	if (!((Serial.read() == 0xff) && (Serial.read() == 0xff))) { //��ʼλ����
		(*ProStruct).an = ProtocolAn_FrameError;
		(*ProStruct).code = 0x00;
		return;
	}
	if (Serial.available() > 20) { //��������
		(*ProStruct).an = ProtocolAn_FrameError;
		(*ProStruct).code = 0x01;
		return;
	}
	uint8_t frameLen = Serial.available();
	Serial.read(Protocol_SendBuf, frameLen);
	if (frameLen != 1 + 1 + Protocol_SendBuf[DATALEN_OFFSET] + 1) { //����֡���Ȳ���
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
	default: //δ֪����
		(*ProStruct).an = ProtocolAn_CommondError;
		return;
	}

	Protocol_Sum = Protocol_SendBuf[DATA_OFFSET + Protocol_SendBuf[DATALEN_OFFSET]]; //��ȡsum
	Protocol_Sum_Check = 0; //����ǰ������
	Protocol_DataLen = Protocol_SendBuf[DATALEN_OFFSET]; //��ȡ���ݳ���
	Protocol_Sum_Check = Protocol_Command + Protocol_DataLen; //����sum
	for (uint8_t i = 0; i < Protocol_SendBuf[DATALEN_OFFSET]; ++i) {
		Protocol_Data[i] = Protocol_SendBuf[DATA_OFFSET + i]; //�������ݲ�����sum
		Protocol_Sum_Check += Protocol_SendBuf[DATA_OFFSET + i];
	}
	if (Protocol_Sum_Check != Protocol_Sum) { //�����sum���յ��Ĳ���
		(*ProStruct).an = ProtocolAn_CheckSumError;
		(*ProStruct).code = Protocol_Sum_Check;
		return;
	}

	uint8_t sendbuf[100];
	uint8_t sendbuflen;
	Protocol_Format(ProtocolCom_Post_Get, Protocol_DataLen, Protocol_Command,
			(uint8_t*) Protocol_Data, sendbuf, &sendbuflen); //��ʽ��ָ���յ��ظ�
	Serial.print(sendbuf, sendbuflen);
	(*ProStruct).an = ProtocolAn_Ok;

	Protocol_Command_Flag = true; //������֤�ɹ�
}

void Protocol_Format(ProtocolCom_Typedef com, uint8_t datalen, uint8_t com_get,
		uint8_t* data, uint8_t* sendbuf, uint8_t* sendbuflen) {
	uint8_t index = 0;
	uint8_t sum = 0;

	sendbuf[index++] = 0xff;
	sendbuf[index++] = 0xff; //��ʼλ
	sendbuf[index++] = com; //ָ��
//�������ݿ�ʼ�������
	sendbuf[index++] = datalen + 1;	//���ݳ���
	sum = com + datalen + 1;
	sendbuf[index++] = com_get;	//�������ݻ��ڵ�ָ��
	sum += com_get;
	for (uint8_t i = 0; i < datalen; ++i) {	//�������
		sendbuf[index++] = *(data + i);
		sum += *(data + i);
	}
	sendbuf[index++] = sum;	//sum
	*sendbuflen = 2 + 1 + 1 + 1 + datalen + 1; //��ʼλ+ָ��+ָ���+�յ���ָ��+�ظ�������+У��
}
