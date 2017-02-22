/*
 * Protocol.cpp
 *
 *  Created on: 2017��1��7��
 *      Author: Romeli
 */

#include "Protocol.h"
#include "U_USART1.h"
#include "TimeTick.h"
#include "ProComFun.h"

#define COMMAND_OFFSET 0
#define DATALEN_OFFSET 1
#define DATA_OFFSET 2

P_Buf_Typedef P_ReceiveBuf;
volatile bool P_ReceiveFlag = false;

P_Buf_Typedef P_RunningBuf;
volatile bool P_RunningFlag = false;

PC_Typedef PC_LastAuto = PC_None;

void Serial_Event() {
	PA_Struct_Typedef pa_struct;
	DataBuf_Typedef sendbuf;

	Protocol_Analysis(&pa_struct); //分析指令
	switch (pa_struct.pa) {
	case PA_Ok: //数据帧无异常
		P_ReceiveBuf.pc = (PC_Typedef) pa_struct.data[COMMAND_OFFSET]; //从缓冲中取得命令
		P_ReceiveBuf.len = pa_struct.data[DATALEN_OFFSET]; //从缓冲中取得数据长度
		for (uint8_t i = 0; i < P_ReceiveBuf.len; ++i) { //从缓冲中搬移出数据
			P_ReceiveBuf.data[i] = pa_struct.data[DATA_OFFSET + i];
		}
		Protocol_Send(PC_Post_Get, P_ReceiveBuf.len, P_ReceiveBuf.pc,
				P_ReceiveBuf.data, &sendbuf);
		P_ReceiveFlag = true; //置位收到新的指令标志
		break;
	case PA_CommondError: //未知指令
		Protocol_Send(PC_Post_CommandError, pa_struct.data[DATALEN_OFFSET],
				pa_struct.data[COMMAND_OFFSET], &pa_struct.data[DATA_OFFSET],
				&sendbuf);
		break;
	case PA_CheckSumError: //ָ校验失败ֵ
		Protocol_Send(PC_Post_CheckSumError, pa_struct.data[DATALEN_OFFSET] + 1,
				pa_struct.data[COMMAND_OFFSET], &pa_struct.data[DATA_OFFSET],
				&sendbuf);
		break;
	case PA_FrameError: //֡帧格式错误
		Protocol_Send(PC_Post_CommandError, 1, pa_struct.data[DATALEN_OFFSET],
				pa_struct.data, &sendbuf);
		break;
	default:
		break;
	}
	Serial.flush();
}

void Protocol_Analysis(PA_Struct_Typedef *pa_struct) {
	if (!((Serial.read() == 0xff) && (Serial.read() == 0xff))) { //起始符错误
		pa_struct->pa = PA_FrameError;
		pa_struct->data[0] = 0xf0;
		return;
	}
	if (Serial.available() > 20) { //帧长度异常
		pa_struct->pa = PA_FrameError;
		pa_struct->data[0] = 0xf1;
		return;
	}
	uint8_t frameLen = Serial.available();
	Serial.read(pa_struct->data, frameLen); //读取帧数据到临时缓冲
	if (frameLen != 1 + 1 + pa_struct->data[DATALEN_OFFSET] + 1) { //判断帧数据长度是否正常
		pa_struct->pa = PA_FrameError;
		pa_struct->data[0] = 0xf2;
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
		pa_struct->data[DATA_OFFSET + pa_struct->data[DATALEN_OFFSET]] =
				p_sumcheck; //最后拼接上正确的校验
		return;
	}

	pa_struct->pa = PA_Ok;
}

void Protocol_Send(PC_Typedef com, uint8_t datalen, uint8_t com_get,
		uint8_t* data, DataBuf_Typedef *sendbuf) {
	uint8_t index = 0;
	uint8_t sum = 0;

	sendbuf->data[index++] = 0xff;
	sendbuf->data[index++] = 0xff; //填充起始字节
	sendbuf->data[index++] = com; //填充指令字节
	sendbuf->data[index++] = datalen + 1;	//填充数据长度字节（指令字节+数据长度）
	sum = com + (datalen + 1);	//先累加指令字节和数据长度字节
	sendbuf->data[index++] = com_get;	//填充收到的指令
	sum += com_get;	//累加校验
	for (uint8_t i = 0; i < datalen; ++i) {	//循环累加数据位，并填充发送缓冲
		sendbuf->data[index++] = *(data + i);
		sum += *(data + i);
	}
	sendbuf->data[index++] = sum;	//sum
	//起始字节2+指令字节1+数据长度字节1+收到的指令1+数据长度+校验
	sendbuf->len = (2 + 1 + 1 + 1 + datalen + 1);
	Serial.print(sendbuf->data, sendbuf->len);
}
