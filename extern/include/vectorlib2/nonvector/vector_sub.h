/*
 * vector_sub.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_SUB_H_
#define VECTOR_SUB_H_


#include "vector_struct.h"

//! Vector Substract
__inline vector_t vector_sub_float (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vf[0] = a.vf[0] - b.vf[0];
	vec.vf[1] = a.vf[1] - b.vf[1];
	vec.vf[2] = a.vf[2] - b.vf[2];
	vec.vf[3] = a.vf[3] - b.vf[3];
	return vec;
}


//! Vector Substract
__inline vector_t vector_sub_sint (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vsi[0] = a.vsi[0] - b.vsi[0];
	vec.vsi[1] = a.vsi[1] - b.vsi[1];
	vec.vsi[2] = a.vsi[2] - b.vsi[2];
	vec.vsi[3] = a.vsi[3] - b.vsi[3];
	return vec;
}

//! Vector Substract
__inline vector_t vector_sub_uint (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vui[0] = a.vui[0] - b.vui[0];
	vec.vui[2] = a.vui[2] - b.vui[2];
	vec.vui[1] = a.vui[1] - b.vui[1];
	vec.vui[3] = a.vui[3] - b.vui[3];
	return vec;
}

//! Vector Substract
__inline vector_t vector_sub_char (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vchar[0] = a.vchar[0] - b.vchar[0];
	vec.vchar[1] = a.vchar[1] - b.vchar[1];
	vec.vchar[2] = a.vchar[2] - b.vchar[2];
	vec.vchar[3] = a.vchar[3] - b.vchar[3];

	vec.vchar[4] = a.vchar[4] - b.vchar[4];
	vec.vchar[5] = a.vchar[5] - b.vchar[5];
	vec.vchar[6] = a.vchar[6] - b.vchar[6];
	vec.vchar[7] = a.vchar[7] - b.vchar[7];

	vec.vchar[8] = a.vchar[8] - b.vchar[8];
	vec.vchar[9] = a.vchar[9] - b.vchar[9];
	vec.vchar[10] = a.vchar[10] - b.vchar[10];
	vec.vchar[11] = a.vchar[11] - b.vchar[11];

	vec.vchar[12] = a.vchar[12] - b.vchar[12];
	vec.vchar[13] = a.vchar[13] - b.vchar[13];
	vec.vchar[14] = a.vchar[14] - b.vchar[14];
	vec.vchar[15] = a.vchar[15] - b.vchar[15];
	return vec;
}

#endif /* VECTOR_SUB_H_ */
