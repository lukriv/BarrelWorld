/*
 * vector_div.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_DIV_H_
#define VECTOR_DIV_H_


#include "vector_struct.h"

//! Vector Divide
__inline vector_t vector_div_float (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vf[0] = a.vf[0] / b.vf[0];
	vec.vf[1] = a.vf[1] / b.vf[1];
	vec.vf[2] = a.vf[2] / b.vf[2];
	vec.vf[3] = a.vf[3] / b.vf[3];
	return vec;
}


#endif /* VECTOR_DIV_H_ */
