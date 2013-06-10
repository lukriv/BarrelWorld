/*
 * vector_sel.h
 *
 *  Created on: 17.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_SEL_H_
#define VECTOR_SEL_H_

#include "vector_struct.h"

//! Vector Reciprocal Value
__inline vector_t vector_sel (vector_t a, vector_t b, vector_t c)
{
	vector_t vec;
	vec.vui[0] = (~(c.vui[0]) & a.vui[0]) | (c.vui[0] & b.vui[0]);
	vec.vui[1] = (~(c.vui[1]) & a.vui[1]) | (c.vui[1] & b.vui[1]);
	vec.vui[2] = (~(c.vui[2]) & a.vui[2]) | (c.vui[2] & b.vui[2]);
	vec.vui[3] = (~(c.vui[3]) & a.vui[3]) | (c.vui[3] & b.vui[3]);
	return vec;
}

#endif /* VECTOR_SEL_H_ */
