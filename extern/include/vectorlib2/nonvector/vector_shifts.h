/*
 * vector_shifts.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_SHIFTS_H_
#define VECTOR_SHIFTS_H_


#include "vector_struct.h"

//! Vector Shift Right Algebraic
__inline vector_t vector_srai_sint (vector_t a, unsigned int sh)
{
	vector_t vec;
	vec.vsi[0] = a.vsi[0] >> sh;
	vec.vsi[1] = a.vsi[1] >> sh;
	vec.vsi[2] = a.vsi[2] >> sh;
	vec.vsi[3] = a.vsi[3] >> sh;
	return vec;
}

//! Vector Shift Left
__inline vector_t vector_sli_sint (vector_t a, unsigned int sh)
{
	vector_t vec;
	vec.vsi[0] = a.vsi[0] << sh;
	vec.vsi[1] = a.vsi[1] << sh;
	vec.vsi[2] = a.vsi[2] << sh;
	vec.vsi[3] = a.vsi[3] << sh;
	return vec;

)




#endif /* VECTOR_SHIFTS_H_ */
