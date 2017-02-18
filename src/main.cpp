//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "main.h"

const uint8_t InitBuf[] = { 0xff, 0xff, 0x40, 0x00, 0x40 };

int main(int argc, char* argv[]) {
	__enable_irq();
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);

	Serial.begin(1024000);
	Delay_Init(50);

	Serial.print((uint8_t*) InitBuf, 5);
	while (1) {
		if (P_Receive_Flag) {
			//���µ�ָ�� ���ƽ��ݴ���
			uint8_t DataBuf[10];
			for (uint8_t i = 0; i < P_Receive_DataLen; ++i) {
				DataBuf[i] = P_Receive_Data[i];
			}

			P_Receive_Flag = false; //�����ָ���־
			P_Running_Flag = true; //��λָ��ִ���б�־
			switch (P_Receive_Command) {
			case PC_Check_Digital:
				Check_Digital();
				break;
			case PC_Check_Analog:
				Check_Analog(DataBuf);
				break;
			case PC_Check_Pressure:
				Check_Pressure();
				break;
			case PC_Check_Flow:
				Check_Flow(DataBuf);
				break;
			case PC_Check_Limit:
				Check_Limit();
				break;
			case PC_Check_Water:
				Check_Water();
				break;
			case PC_Check_Stepper:
				Check_Stepper(DataBuf);
				break;
			case PC_Contrl_Valve:
				Contrl_Valve(DataBuf);
				break;
			case PC_Contrl_Motor:
				Contrl_Motor(DataBuf);
				break;
			case PC_AutoContrl_Valve_With_Flow:
				TimeTick.Enable();
				AutoContrl_Valve_With_Flow(DataBuf);
				break;
			case PC_AutoContrl_Motor_With_Limit:
				TimeTick.Enable();
				AutoContrl_Motor_With_Limit(DataBuf);
				break;
			case PC_AutoContrl_Motor_With_Time:
				TimeTick.Enable();
				AutoContrl_Motor_With_Time(DataBuf);
				break;
			case PC_AutoContrl_Stepper_With_Limit:
				TimeTick.Enable();
				AutoContrl_Stepper_With_Limit(DataBuf);
				break;
			case PC_AutoContrl_Stepper_With_Presure:
				TimeTick.Enable();
				AutoContrl_Stepper_With_Presure(DataBuf);
				break;
			case PC_AutoContrl_Stepper_With_Step:
				TimeTick.Enable();
				AutoContrl_Stepper_With_Step(DataBuf);
				break;
			case PC_AutoContrl_Stepper_With_Position:
				TimeTick.Enable();
				AutoContrl_Stepper_With_Position(DataBuf);
				break;
			case PC_Setting_ClearFlow:
				Setting_ClearFlow();
				break;
			case PC_Setting_SetStepperLimit:
				Setting_SetStepperLimit(DataBuf);
				break;
			case PC_Setting_ClearStepperPositon:
				Setting_ClearStepperPosition(DataBuf);
				break;
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
			default:
				break;
			}
			P_Running_Flag = false;
			TimeTick.Disable();
		}
	}
}

void TimeTickISR() {
	if ((P_Running_Flag == true) && (P_Receive_Flag == true)) { //�յ��µ�ָ���������ִ���Զ�����

		//���ƽ��ݴ���
		uint8_t DataBuf[10];
		for (uint8_t i = 0; i < P_Receive_DataLen; ++i) {
			DataBuf[i] = P_Receive_Data[i];
		}

		switch (P_Receive_Command) {
		case PC_Check_Digital:
			Check_Digital();
			break;
//		case PC_Check_Analog:
//			Check_Analog(DataBuf);
//			break;
		case PC_Check_Pressure:
			Check_Pressure();
			break;
		case PC_Check_Flow:
			Check_Flow(DataBuf);
			break;
		case PC_Check_Limit:
			Check_Limit();
			break;
		case PC_Check_Water:
			Check_Water();
			break;
		case PC_Check_Stepper:
			Check_Stepper(DataBuf);
			break;
		case PC_Contrl_Valve:
			Contrl_Valve(DataBuf);
			break;
		case PC_Contrl_Motor:
			Contrl_Motor(DataBuf);
			break;
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
		default:
			uint8_t SendBuf[100];
			uint8_t SendBufLen = 0;

			SendBufLen = Protocol_Format(PC_Post_Busy, P_Receive_DataLen,
					P_Receive_Command, P_Receive_Data, SendBuf);
			Serial.print(SendBuf, SendBufLen);
			P_Receive_Flag = false;
			break;
		}
		P_Receive_Flag = false;
	}
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
