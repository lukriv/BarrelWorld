/*
 * vector_rsqrt.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_RSQRT_H_
#define VECTOR_RSQRT_H_


#include "vector_struct.h"

#include <cmath>
//! Vector Reciprocal Square Roots
__inline vector_t vector_rsqrt_float (vector_t a)
{
	vector_t vec;
	vec.vf[0] = 1.0 / sqrt(a.vf[0]);
	vec.vf[1] = 1.0 / sqrt(a.vf[1]);
	vec.vf[2] = 1.0 / sqrt(a.vf[2]);
	vec.vf[3] = 1.0 / sqrt(a.vf[3]);
	return vec;
}

#endif /* VECTOR_RSQRT_H_ */
