/*
 * vector_extract.h
 *
 *  Created on: 8.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTOR_EXTRACT_H_
#define VECTOR_EXTRACT_H_

#include "vector_struct.h"

//! Extract vector element from vector
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns selected element.
 */
__inline float vector_extract_float (vector_t a, int element)
{
	element = element & 3;
	return a.vf[element];
}

//! Extract vector element from vector
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns selected element.
 */
__inline int vector_extract_sint (vector_t a, int element)
{
	element = element & 3;
	return a.vsi[element];
}

//! Extract vector element from vector
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns selected element.
 */
__inline unsigned int vector_extract_uint (vector_t a, int element)
{
	element = element & 3;
	return a.vui[element];
}

//! Extract vector element from vector
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns selected element.
 */
__inline char vector_extract_char (vector_t a, int element)
{
	element = element & 0x0f;
	return a.vchar[element];
}

#endif /* VECTOR_EXTRACT_H_ */
