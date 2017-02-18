/*
 * Protocol.cpp
 *
 *  Created on: 2017��1��7��
 *      Author: Romeli
 */

#include "Protocol.h"
#include "U_USART1.h"

#define COMMAND_OFFSET 0
#define DATALEN_OFFSET 1
#define DATA_OFFSET 2

PC_Typedef P_Receive_Command = PC_None;
uint8_t P_Receive_Data[20];
uint8_t P_Receive_DataLen = 0;

volatile bool P_Receive_Flag = false;
volatile bool P_Running_Flag = false;

void Serial_Event() {
	PA_Struct_Typedef pa_struct;
	uint8_t buftmp[20];
	uint8_t sendbuf[20];
	uint8_t sendbuflen = 0;

	Protocol_Analysis(&pa_struct); //分析指令
	switch (pa_struct.pa) {
	case PA_Ok: //数据帧无异常
		P_Receive_Command = (PC_Typedef) pa_struct.data[COMMAND_OFFSET]; //从缓冲中取得命令
		P_Receive_DataLen = pa_struct.data[DATALEN_OFFSET]; //从缓冲中取得数据长度
		for (uint8_t i = 0; i < P_Receive_DataLen; ++i) { //从缓冲中搬移出数据
			P_Receive_Data[i] = pa_struct.data[DATA_OFFSET + i];
		}
		sendbuflen = Protocol_Format(PC_Post_Get, P_Receive_DataLen + 1,
				P_Receive_Command, P_Receive_Data, sendbuf);
		Serial.print(sendbuf, sendbuflen);
		P_Receive_Flag = true; //置位收到新的指令标志
		break;
	case PA_CommondError: //未知指令
		for (uint8_t i = 0; i < pa_struct.data[DATALEN_OFFSET]; ++i) {
			buftmp[i] = pa_struct.data[DATA_OFFSET + i];
		}
		sendbuflen = Protocol_Format(PC_Post_CommandError,
				pa_struct.data[DATALEN_OFFSET], pa_struct.data[COMMAND_OFFSET],
				buftmp, sendbuf);
		Serial.print(sendbuf, sendbuflen);
		break;
	case PA_CheckSumError: //ָ校验失败
		uint8_t i;
		for (i = 0; i < pa_struct.data[DATALEN_OFFSET]; ++i) {
			sendbuf[i] = pa_struct.data[DATA_OFFSET + i];
		}
		sendbuf[i++] = pa_struct.code; //添加上返回值ֵ
		sendbuflen = Protocol_Format(PC_Post_CheckSumError,
				pa_struct.data[DATALEN_OFFSET] + 1,
				pa_struct.data[COMMAND_OFFSET], buftmp, sendbuf);
		Serial.print(sendbuf, sendbuflen);
		break;
	case PA_FrameError: //֡帧格式错误
		sendbuflen = Protocol_Format(PC_Post_CommandError, 1,
				pa_struct.data[DATALEN_OFFSET], &pa_struct.code, sendbuf);
		Serial.print(sendbuf, sendbuflen);
		break;
	default:
		break;
	}
}

void Protocol_Analysis(PA_Struct_Typedef *pa_struct) {
	if (!((Serial.read() == 0xff) && (Serial.read() == 0xff))) { //起始符错误
		pa_struct->pa = PA_FrameError;
		pa_struct->code = 0x00;
		return;
	}
	if (Serial.available() > 20) { //帧长度异常
		pa_struct->pa = PA_FrameError;
		pa_struct->code = 0x01;
		return;
	}
	uint8_t frameLen = Serial.available();
	Serial.read(pa_struct->data, frameLen); //读取帧数据到临时缓冲
	if (frameLen != 1 + 1 + pa_struct->data[DATALEN_OFFSET] + 1) { //判断帧数据长度是否正常
		pa_struct->pa = PA_FrameError;
		pa_struct->code = 0x02;
		return;
	}

	switch (pa_struct->data[COMMAND_OFFSET]) { //判断指令字节是否有效
	case PC_Check_Digital:
	case PC_Check_Analog:
	case PC_Check_Pressure:
	case PC_Check_Flow:
	case PC_Check_Limit:
	case PC_Check_Water:
	case PC_Check_Stepper:
	case PC_Contrl_Valve:
	case PC_Contrl_Motor:
	case PC_AutoContrl_Valve_With_Flow:
	case PC_AutoContrl_Motor_With_Limit:
	case PC_AutoContrl_Motor_With_Time:
	case PC_AutoContrl_Stepper_With_Limit:
	case PC_AutoContrl_Stepper_With_Presure:
	case PC_AutoContrl_Stepper_With_Step:
	case PC_AutoContrl_Stepper_With_Position:
	case PC_Setting_ClearFlow:
	case PC_Setting_SetStepperLimit:
	case PC_Setting_ClearStepperPositon:
	case PC_Special_Reset:
	case PC_Special_Stop:
	case PC_Special_Continue:
	case PC_Special_Online:
	case PC_Special_Cacel:
	case PC_Special_BootLoader:
		break;
	default: //未知指令
		pa_struct->pa = PA_CommondError;
		return;
	}
	PC_Typedef p_receive_command;
	uint8_t p_receive_datalen;
	uint8_t p_sum, p_sumcheck = 0;
	p_receive_command = (PC_Typedef) pa_struct->data[COMMAND_OFFSET];
	p_receive_datalen = pa_struct->data[DATALEN_OFFSET]; //根据偏移量取得数据长度

	p_sum = pa_struct->data[DATA_OFFSET + pa_struct->data[DATALEN_OFFSET]]; //根据偏移量取得校验码
	p_sumcheck = p_receive_command + p_receive_datalen; //先累加指令字节和数据长度字节
	for (uint8_t i = 0; i < p_receive_datalen; ++i) {
		p_sumcheck += pa_struct->data[DATA_OFFSET + i]; //累加数据字节
	}
	if (p_sumcheck != p_sum) { //取得的校验字节和计算不一致，校验不通过
		pa_struct->pa = PA_CheckSumError;
		pa_struct->code = p_sumcheck;
		return;
	}
	for (uint8_t i = 0; i < p_receive_datalen; i++) {
	}

	pa_struct->pa = PA_Ok;
}

uint8_t Protocol_Format(PC_Typedef com, uint8_t datalen, uint8_t com_get,
		uint8_t* data, uint8_t* sendbuf) {
	uint8_t index = 0;
	uint8_t sum = 0;

	sendbuf[index++] = 0xff;
	sendbuf[index++] = 0xff; //填充起始字节
	sendbuf[index++] = com; //填充指令字节
	sendbuf[index++] = datalen + 1;	//填充数据长度字节（指令字节+数据长度）
	sum = com + (datalen + 1);	//先累加指令字节和数据长度字节
	sendbuf[index++] = com_get;	//填充收到的指令
	sum += com_get;	//累加校验
	for (uint8_t i = 0; i < datalen; ++i) {	//循环累加数据位，并填充发送缓冲
		sendbuf[index++] = *(data + i);
		sum += *(data + i);
	}
	sendbuf[index++] = sum;	//sum
	//起始字节2+指令字节1+数据长度字节1+收到的指令1+数据长度+校验
	return (2 + 1 + 1 + 1 + datalen + 1);
}
