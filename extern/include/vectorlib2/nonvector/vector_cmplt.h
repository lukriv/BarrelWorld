/*
 * vector_cmplt.h
 *
 *  Created on: 24.3.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_CMPLT_H_
#define VECTOR_CMPLT_H_

#include "vector_struct.h"

//! Vector Compare Equal
__inline vector_t vector_cmplt_float (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vui[0] = (a.vf[0] < b.vf[0]) ? -1 : 0;
	vec.vui[1] = (a.vf[1] < b.vf[1]) ? -1 : 0;
	vec.vui[2] = (a.vf[2] < b.vf[2]) ? -1 : 0;
	vec.vui[3] = (a.vf[3] < b.vf[3]) ? -1 : 0;
	return vec;
}

//! Vector Compare Equal
__inline vector_t vector_cmplt_sint (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vui[0] = (a.vsi[0] < b.vsi[0]) ? -1 : 0;
	vec.vui[1] = (a.vsi[1] < b.vsi[1]) ? -1 : 0;
	vec.vui[2] = (a.vsi[2] < b.vsi[2]) ? -1 : 0;
	vec.vui[3] = (a.vsi[3] < b.vsi[3]) ? -1 : 0;
	return vec;
}

//! Vector Compare Equal
__inline vector_t vector_cmplt_uint (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vui[0] = (a.vui[0] < b.vui[0]) ? -1 : 0;
	vec.vui[1] = (a.vui[1] < b.vui[1]) ? -1 : 0;
	vec.vui[2] = (a.vui[2] < b.vui[2]) ? -1 : 0;
	vec.vui[3] = (a.vui[3] < b.vui[3]) ? -1 : 0;
	return vec;
}

//! Vector Compare Equal
__inline vector_t vector_cmplt_char (vector_t a, vector_t b)
{
	vector_t vec;
	vec.vchar[0] = (a.vchar[0] < b.vchar[0]) ? -1 : 0;
	vec.vchar[1] = (a.vchar[1] < b.vchar[1]) ? -1 : 0;
	vec.vchar[2] = (a.vchar[2] < b.vchar[2]) ? -1 : 0;
	vec.vchar[3] = (a.vchar[3] < b.vchar[3]) ? -1 : 0;

	vec.vchar[4] = (a.vchar[4] < b.vchar[4]) ? -1 : 0;
	vec.vchar[5] = (a.vchar[5] < b.vchar[5]) ? -1 : 0;
	vec.vchar[6] = (a.vchar[6] < b.vchar[6]) ? -1 : 0;
	vec.vchar[7] = (a.vchar[7] < b.vchar[7]) ? -1 : 0;

	vec.vchar[8] = (a.vchar[8] < b.vchar[8]) ? -1 : 0;
	vec.vchar[9] = (a.vchar[9] < b.vchar[9]) ? -1 : 0;
	vec.vchar[10] = (a.vchar[10] < b.vchar[10]) ? -1 : 0;
	vec.vchar[11] = (a.vchar[11] < b.vchar[11]) ? -1 : 0;

	vec.vchar[12] = (a.vchar[12] < b.vchar[12]) ? -1 : 0;
	vec.vchar[13] = (a.vchar[13] < b.vchar[13]) ? -1 : 0;
	vec.vchar[14] = (a.vchar[14] < b.vchar[14]) ? -1 : 0;
	vec.vchar[15] = (a.vchar[15] < b.vchar[15]) ? -1 : 0;
	return vec;
}

#endif /* VECTOR_CMPLT_H_ */
