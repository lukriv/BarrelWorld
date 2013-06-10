/*
 * vector_insert.h
 *
 *  Created on: 15.2.2010
 *      Author: Lukáš
 */

#ifndef VECTOR_INSERT_H_
#define VECTOR_INSERT_H_


#if defined(__SSE2__)
#include <emmintrin.h>



//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns __m128.
 */
__inline __m128 vector_insert( float scal, __m128 vec, int elem)
{
	union {
		float v[4];
		__m128 vec;
	} temp;

	elem &= 3;
	temp.vec = vec;
	temp.v[elem] = scal;

	return temp.vec;

}


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns __m128i.
 */
__inline __m128i vector_insert( int scal, __m128i vec, int elem)
{
	union {
		int v[4];
		__m128i vec;
	} temp;

	elem &= 3;
	temp.vec = vec;
	temp.v[elem] = scal;

	return temp.vec;

}


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns __m128i.
 */
__inline __m128i vector_insert( unsigned int scal, __m128i vec, int elem)
{
	union {
		unsigned int v[4];
		__m128i vec;
	} temp;

	elem &= 3;
	temp.vec = vec;
	temp.v[elem] = scal;

	return temp.vec;

}


//! Insert scalar into specified vector element
/*!
 * From parameter element there will be used only the least significant bit to chose vector element.
 * \param scalar Input scalar
 * \param vec Input vector
 * \param element Number of selected element
 *
 * \return It returns __m128i.
 */
__inline __m128i vector_insert( char scal, __m128i vec, int elem)
{
	union {
		char v[16];
		__m128i vec;
	} temp;

	elem &= 0x0f;
	temp.vec = vec;
	temp.v[elem] = scal;

	return temp.vec;

}
#else /*!__SSE2__ */
#error "SSE2 extension not detected"
#endif

#endif /* VECTOR_INSERT_H_ */
