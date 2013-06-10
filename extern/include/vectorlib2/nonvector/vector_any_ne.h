/*
 * vector_any_ne.h
 *
 *  Created on: 2.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_ANY_NE_H_
#define VECTOR_ANY_NE_H_

#include "vector_struct.h"

__inline int vector_any_ne_float(vector_t a, vector_t b)
{
	int temp = 0;
	temp = (a.vf[0] != b.vf[0]) ? 1 : 0;
	temp |= (a.vf[1] != b.vf[1]) ? 2 : 0;
	temp |= (a.vf[2] != b.vf[2]) ? 4 : 0;
	temp |= (a.vf[3] != b.vf[3]) ? 8 : 0;
	return (temp != 0);
}


__inline int vector_any_ne_sint(vector_t a, vector_t b)
{
	int temp = 0;
	temp = (a.vsi[0] != b.vsi[0]) ? 1 : 0;
	temp |= (a.vsi[1] != b.vsi[1]) ? 2 : 0;
	temp |= (a.vsi[2] != b.vsi[2]) ? 4 : 0;
	temp |= (a.vsi[3] != b.vsi[3]) ? 8 : 0;
	return (temp != 0);
}

__inline int vector_any_ne_uint(vector_t a, vector_t b)
{
	int temp = 0;
	temp = (a.vui[0] != b.vui[0]) ? 1 : 0;
	temp |= (a.vui[1] != b.vui[1]) ? 2 : 0;
	temp |= (a.vui[2] != b.vui[2]) ? 4 : 0;
	temp |= (a.vui[3] != b.vui[3]) ? 8 : 0;
	return (temp != 0);
}

__inline int vector_any_ne_char(vector_t a, vector_t b)
{
	int temp = 0;
	temp = (a.vchar[0] != b.vchar[0]) ? 0x1 : 0;
	temp |= (a.vchar[1] != b.vchar[1]) ? 0x2 : 0;
	temp |= (a.vchar[2] != b.vchar[2]) ? 0x4 : 0;
	temp |= (a.vchar[3] != b.vchar[3]) ? 0x8 : 0;

	temp |= (a.vchar[4] != b.vchar[4]) ? 0x10 : 0;
	temp |= (a.vchar[5] != b.vchar[5]) ? 0x20 : 0;
	temp |= (a.vchar[6] != b.vchar[6]) ? 0x40 : 0;
	temp |= (a.vchar[7] != b.vchar[7]) ? 0x80 : 0;

	temp |= (a.vchar[8] != b.vchar[8]) ? 0x100 : 0;
	temp |= (a.vchar[9] != b.vchar[9]) ? 0x200 : 0;
	temp |= (a.vchar[10] != b.vchar[10]) ? 0x400 : 0;
	temp |= (a.vchar[11] != b.vchar[11]) ? 0x800 : 0;

	temp |= (a.vchar[12] != b.vchar[12]) ? 0x1000 : 0;
	temp |= (a.vchar[13] != b.vchar[13]) ? 0x2000 : 0;
	temp |= (a.vchar[14] != b.vchar[14]) ? 0x4000 : 0;
	temp |= (a.vchar[15] != b.vchar[15]) ? 0x8000 : 0;
	return (temp != 0);
}

#endif /* VECTOR_ANY_NE_H_ */
