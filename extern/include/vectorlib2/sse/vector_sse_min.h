/*
 * vector_sse_min.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_SSE_MIN_H_
#define VECTOR_SSE_MIN_H_

#if defined(__SSE2__)
#include <emmintrin.h>

__inline __m128i _sse_min_sint (__m128i a, __m128i b)
{
	__m128i t;
	t = _mm_cmplt_epi32(a,b);
	return _mm_or_si128(_mm_andnot_si128(t,b),_mm_and_si128(t,a));
}

__inline __m128i _sse_min_uint (__m128i a, __m128i b)
{
	__m128i t;
	t = _mm_cmplt_epi32(_mm_add_epi32(a,_mm_set1_epi32(0x80000000)),_mm_add_epi32(b,_mm_set1_epi32(0x80000000)));
	return _mm_or_si128(_mm_andnot_si128(t,b),_mm_and_si128(t,a));
}

__inline __m128i _sse_min_char (__m128i a, __m128i b)
{
	__m128i t;
	t = _mm_cmplt_epi8(a,b);
	return _mm_or_si128(_mm_andnot_si128(t,b),_mm_and_si128(t,a));
}

#else /*!__SSE2__ */
#error SSE2 extension not detected
#endif

#endif /* VECTOR_SSE_MIN_H_ */
