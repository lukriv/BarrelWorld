/*
 * vector_insert.h
 *
 *  Created on: 8.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTOR_INSERT_H_
#define VECTOR_INSERT_H_

#include "vector_struct.h"


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns vector_t.
 */
__inline vector_t vector_insert (float scalar, vector_t vec, int element)
{
	element = element & 3;
	vec.vf[element] = scalar;
	return vec;
}


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns vector_t.
 */
__inline vector_t vector_insert (int scalar, vector_t vec, int element)
{
	element = element & 3;
	vec.vsi[element] = scalar;
	return vec;
}


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns vector_t.
 */
__inline vector_t vector_insert (unsigned int scalar, vector_t vec, int element)
{
	element = element & 3;
	vec.vui[element] = scalar;
	return vec;
}


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns vector_t.
 */
__inline vector_t vector_insert ( char scalar, vector_t vec, int element)
{
	element = element & 0x0f;
	vec.vchar[element] = scalar;
	return vec;
}

#endif /* VECTOR_INSERT_H_ */
