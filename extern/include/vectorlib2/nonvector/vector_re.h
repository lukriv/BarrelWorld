/*
 * vector_re.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_RE_H_
#define VECTOR_RE_H_

#include "vector_struct.h"

//! Vector Reciprocal Value
__inline vector_t vector_re_float (vector_t a)
{
	vector_t vec;
	vec.vf[0] = 1.0 / a.vf[0];
	vec.vf[1] = 1.0 / a.vf[1];
	vec.vf[2] = 1.0 / a.vf[2];
	vec.vf[3] = 1.0 / a.vf[3];
	return vec;
}

#endif /* VECTOR_RE_H_ */
