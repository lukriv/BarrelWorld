/*
 * vector_cvt.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_CVT_H_
#define VECTOR_CVT_H_


#include "vector_struct.h"

//! Vector Convert Float to Signed Int Truncate
__inline vector_t vector_cvt_float_sint (vector_t a)
{
	vector_t vec;
	vec.vsi[0] = (int) a.vf[0];
	vec.vsi[1] = (int) a.vf[1];
	vec.vsi[2] = (int) a.vf[2];
	vec.vsi[3] = (int) a.vf[3];
	return vec;
}


//! Vector Convert Signed Int to Float
__inline vector_t vector_cvt_sint_float (vector_t a)
{
	vector_t vec;
	vec.vf[0] = (float) a.vsi[0];
	vec.vf[1] = (float) a.vsi[1];
	vec.vf[2] = (float) a.vsi[2];
	vec.vf[3] = (float) a.vsi[3];
	return vec;
}

//! Vector Convert Float to Unsigned Int Truncate
__inline vector_t vector_cvt_float_uint (vector_t a)
{
	vector_t vec;
	vec.vui[0] = (unsigned int) a.vf[0];
	vec.vui[1] = (unsigned int) a.vf[1];
	vec.vui[2] = (unsigned int) a.vf[2];
	vec.vui[3] = (unsigned int) a.vf[3];
	return vec;
}

#endif /* VECTOR_CVT_H_ */
