/*
 * vector_extract.h
 *
 *  Created on: 12.2.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_EXTRACT_H_
#define VECTOR_EXTRACT_H_


#if defined(__SSE2__)

#include <emmintrin.h>


//! Extract vector element from vector
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param a Input vector
 * \param element Number of selected element
 *
 * \return It returns selected element.
 */
__inline float vector_extract_float( __m128 vec, int elem)
{
	union
	{
		float v[4];
		__m128 vec;
	} temp;

	temp.vec = vec;
	elem &= 3;
	return temp.v[elem];

}

__inline int vector_extract_sint( __m128i vec, int elem)
{
	union
	{
		signed int v[4];
		__m128i vec;
	} temp;

	temp.vec = vec;
	elem &= 3;
	return temp.v[elem];

}

__inline unsigned int vector_extract_uint( __m128i vec, int elem)
{
	union
	{
		unsigned int v[4];
		__m128i vec;
	} temp;

	temp.vec = vec;
	elem &= 3;
	return temp.v[elem];
}

__inline char vector_extract_char( __m128i vec, int elem)
{
	union
	{
		char v[16];
		__m128i vec;
	} temp;

	temp.vec = vec;
	elem &= 0x0f;
	return temp.v[elem];

}
#else /*!__SSE2__ */
#error "SSE2 extension not detected"
#endif

#endif /* VECTOR_EXTRACT_H_ */
