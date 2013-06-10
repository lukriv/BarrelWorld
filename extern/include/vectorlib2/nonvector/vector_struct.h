/*
 * vector_struct.h
 *
 *  Created on: 8.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTOR_STRUCT_H_
#define VECTOR_STRUCT_H_

	#include <stdint.h>

	typedef union vector_t_def {
		float vf[4];
		int32_t vsi[4];
		uint32_t vui[4];
		int8_t vchar[16];
	}vector_t;

#endif /* VECTOR_STRUCT_H_ */
