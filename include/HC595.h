/*
 * HC595.h
 *
 *  Created on: 2016年12月24日
 *      Author: Romeli
 *      PinMap:	|DS-----PC5|数据输入
 *      		|OE-----PE7|低电平有效
 *      		|STCP---PC6|存储寄存器的时钟输入
 *      		|SHCP---PC7|移位寄存器的时钟输入
 */

#ifndef HC595_H_
#define HC595_H_

#include "stm32f10x.h"

class HC595Class {
public:
	HC595Class(GPIO_TypeDef* GPIOx, uint16_t DS, uint16_t OE, uint16_t STCP,
			uint16_t SHCP);
	void Write(uint16_t data);
	inline void Disable();
	inline void Enable();

private:
	void GPIOInit();
	inline void Delay();
	GPIO_TypeDef* GPIOPort;
	uint16_t DS_PIN;
	uint16_t OE_PIN;
	uint16_t STCP_PIN;
	uint16_t SHCP_PIN;
};

#endif /* HC595_H_ */
