/*
 * vector_sqrt.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_SQRT_H_
#define VECTOR_SQRT_H_


#include "vector_struct.h"

#include <cmath>
//! Vector Square Roots
__inline vector_t vector_sqrt_float (vector_t a)
{
	vector_t vec;
	vec.vf[0] = sqrt(a.vf[0]);
	vec.vf[1] = sqrt(a.vf[1]);
	vec.vf[2] = sqrt(a.vf[2]);
	vec.vf[3] = sqrt(a.vf[3]);
	return vec;
}



#endif /* VECTOR_SQRT_H_ */
