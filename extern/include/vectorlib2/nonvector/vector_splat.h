/*
 * vector_splat.h
 *
 *  Created on: 8.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTOR_SPLAT_H_
#define VECTOR_SPLAT_H_

#include "vector_struct.h"

//! Vector Splat
/*!
 * Each element of the result is component b of a.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns vector with selected component.
 */
__inline vector_t vector_splat_float (vector_t a, int element)
{
	float temp;	
	element = element & 3;
	temp = a.vf[element];
	
	a.vf[0] = temp;
	a.vf[1] = temp;
	a.vf[2] = temp;
	a.vf[3] = temp;
	return a;
}

//! Vector Splat
/*!
 * Each element of the result is component b of a.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns vector with selected component.
 */
__inline vector_t vector_splat_sint (vector_t a, int element)
{
	int temp;	
	element = element & 3;
	temp = a.vsi[element];
	
	a.vsi[0] = temp;
	a.vsi[1] = temp;
	a.vsi[2] = temp;
	a.vsi[3] = temp;
	return a;
}

//! Vector Splat
/*!
 * Each element of the result is component b of a.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns vector with selected component.
 */
__inline vector_t vector_splat_uint (vector_t a, int element)
{
	unsigned int temp;
	element = element & 3;
	temp = a.vui[element];

	a.vui[0] = temp;
	a.vui[1] = temp;
	a.vui[2] = temp;
	a.vui[3] = temp;
	return a;
}

//! Vector Splat
/*!
 * Each element of the result is component b of a.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns vector with selected component.
 */
__inline vector_t vector_splat_char (vector_t a, int element)
{
	char temp;	
	element = element & 0x0f;
	temp = a.vchar[element];

	a.vchar[0] = temp;
	a.vchar[1] = temp;
	a.vchar[2] = temp;
	a.vchar[3] = temp;

	a.vchar[4] = temp;
	a.vchar[5] = temp;
	a.vchar[6] = temp;
	a.vchar[7] = temp;

	a.vchar[8] = temp;
	a.vchar[9] = temp;
	a.vchar[10] = temp;
	a.vchar[11] = temp;

	a.vchar[12] = temp;
	a.vchar[13] = temp;
	a.vchar[14] = temp;
	a.vchar[15] = temp;
	return a;

}

#endif /* VECTOR_SPLAT_H_ */
