/*
 * vector_sse_splat.h
 *
 *  Created on: 12.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_SSE_SPLAT_H_
#define VECTOR_SSE_SPLAT_H_


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
__inline __m128 vector_splat_float( __m128 vec, int elem)
{
	union
	{
		float v[4];
		__m128 vec;
	} temp;

	temp.vec = vec;
	elem &= 3;
	return _mm_set1_ps(temp.v[elem]);

}

__inline __m128i vector_splat_sint( __m128i vec, int elem)
{
	union
	{
		signed int v[4];
		__m128i vec;
	} temp;

	temp.vec = vec;
	elem &= 3;
	return _mm_set1_epi32(temp.v[elem]);

}

__inline __m128i vector_splat_uint( __m128i vec, int elem)
{
	union
	{
		unsigned int v[4];
		__m128i vec;
	} temp;

	temp.vec = vec;
	elem &= 3;
	return _mm_set1_epi32(temp.v[elem]);
}

__inline __m128i vector_splat_char( __m128i vec, int elem)
{
	union
	{
		char v[16];
		__m128i vec;
	} temp;

	temp.vec = vec;
	elem &= 0x0f;
	return _mm_set1_epi8(temp.v[elem]);

}
#else /*!__SSE2__ */
#error "SSE2 extension not detected"
#endif

#endif /* VECTOR_SSE_SPLAT_H_ */
