#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "main.h"

const uint8_t InitBuf[] = { 0xff, 0xff, 0xc4, 0x00, 0xc4 };

int main(int argc, char* argv[]) {
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
	__enable_irq();

	Serial.begin(1024000);
	Delay_Init(50);

	Serial.print((uint8_t*) InitBuf, 5);
	while (1) {
		if (P_ReceiveFlag) {
			P_Buf_Typedef p_buf;
			//从已接受的指令缓冲中搬移到执行缓冲
			p_buf.pc = P_ReceiveBuf.pc;
			for (uint8_t i = 0; i < P_ReceiveBuf.len; ++i) {
				p_buf.data[i] = P_ReceiveBuf.data[i];
			}
			P_ReceiveFlag = false; //清零新指令标准

			P_RunningFlag = true; //置位指令执行中标志
			switch (p_buf.pc & PC_Mask) { //根据Mask选择大类
			case PC_Check_Mask:
				PC_Check(&p_buf);
				break;
			case PC_Contrl_Mask:
				PC_Contrl(&p_buf);
				break;
			case PC_AutoContrl_Mask:
				TimeTick.ThreadStart = true;
				PC_AutoContrl(&p_buf); //自动执行过程中开启巡查
				TimeTick.ThreadStart = false;
				break;
			case PC_Setting_Mask:
				PC_Setting(&p_buf);
				break;
			case PC_Special_Mask:
				PC_Special(&p_buf);
				break;
			default:
				break;
			}
			P_RunningFlag = false; //清零指令执行中标志
		}
	}
}

void TimeTickISR() {
	//指令执行中并且有新的指令
	if ((P_RunningFlag == true) && (P_ReceiveFlag == true)) {
		P_Buf_Typedef p_buf;
		//从已接受的指令缓冲中搬移到执行缓冲
		p_buf.pc = P_ReceiveBuf.pc;
		for (uint8_t i = 0; i < P_ReceiveBuf.len; ++i) {
			p_buf.data[i] = P_ReceiveBuf.data[i];
		}
		if ((p_buf.pc & PC_Mask) == PC_Special_Mask) {
			PC_Special(&p_buf);
		} else if (TimeTick.ThreadStart
				&& (((p_buf.pc & PC_Mask) == PC_Check_Mask)
						&& (p_buf.pc != PC_Check_Analog))) {
			//查询指令，并且不是湿度监测指令
			PC_Check(&p_buf);
		} else { //返回忙
			DataBuf_Typedef sendbuf;
			Protocol_Format(PC_Post_Busy, p_buf.len, p_buf.pc, p_buf.data,
					&sendbuf);
			Serial.print(sendbuf.data, sendbuf.len);
		}
		P_ReceiveFlag = false;
	}
}
#pragma GCC diagnostic pop
