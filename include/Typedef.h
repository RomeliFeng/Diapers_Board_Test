/*
 * Typedef.h
 *
 *  Created on: 2017Äê1ÔÂ8ÈÕ
 *      Author: Romeli
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

typedef union _WordtoByte_Typedef {
	uint8_t byte[2];
	uint16_t word;
} WordtoByte_Typedef;

typedef union _TwoWordtoByte_Typedef {
	uint8_t byte[4];
	uint32_t twoword;
} TwoWordtoByte_Typedef;

typedef union _TwoWordtoByteSigned_Typedef {
	uint8_t byte[4];
	int32_t twoword;
} TwoWordtoByteSigned_Typedef;

#endif /* TYPEDEF_H_ */
