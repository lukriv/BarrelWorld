/*
 * vectormisc.h
 *
 *  Created on: 17.4.2010
 *      Author: Lukáš
 */

#ifndef VECTORMISC_H_
#define VECTORMISC_H_

/*! \def VECTOR_SEL_FLOAT(a,b,c)
 *
 *	\brief Vector Select
 *
 * Each bit of the result is the corresponding bit of a if the corresponding bit of c is 0.
 * Otherwise, it is the corresponding bit of b. \a a and \a b are VECTOR_FLOAT type, \a c is VECTOR_UINT type.
 *
 */

/*! \def VECTOR_SEL_SINT(a,b,c)
 *
 *	\brief Vector Select
 *
 * Each bit of the result is the corresponding bit of a if the corresponding bit of c is 0.
 * Otherwise, it is the corresponding bit of b. \a a and \a b are VECTOR_SINT type, \a c is VECTOR_UINT type.
 *
 */


/*! \def VECTOR_SEL_UINT(a,b,c)
 *
 *	\brief Vector Select
 *
 * Each bit of the result is the corresponding bit of a if the corresponding bit of c is 0.
 * Otherwise, it is the corresponding bit of b. \a a and \a b are VECTOR_UINT type, \a c is VECTOR_UINT type.
 *
 */

/*! \def VECTOR_SEL_CHAR(a,b,c)
 *
 *	\brief Vector Select
 *
 * Each bit of the result is the corresponding bit of a if the corresponding bit of c is 0.
 * Otherwise, it is the corresponding bit of b. \a a and \a b are VECTOR_CHAR type, \a c is VECTOR_UINT type.
 *
 */

/*! \def VECTOR_CVT_FLOAT_SINT(a)
 * 	
 *  \brief Vector Convert Float to Signed integer (truncate)
 * 
 * Each float element is truncated to signed integer. Input type is VECTOR_FLOAT, output type is VECTOR_SINT.
 */

/*! \def VECTOR_CVT_SINT_FLOAT(a)
 * 	
 *  \brief Vector Convert Signed integer to Float
 *  
 * Each signed integer element is converted to float. Input type is VECTOR_SINT, output type is VECTOR_FLOAT.
 */

/*! \def VECTOR_SRAI_SINT(a,sh)
 * 	
 *  \brief Vector Shift Right Algebraic
 *  
 * Shifts the 4 signed 32-bit integers in a right by count bits while shifting in the sign bit. \a a is VECTOR_SINT type \a sh is standard int type.
 * (For nonvector use only)
 */

/*! \def VECTOR_SLI_SINT(a,sh)
 * 	
 *  \brief Vector Shift Left
 *  
 * Shifts the 4 signed 32-bit integers in a left by count bits. \a a is VECTOR_SINT type \a sh is standard int type.
 * (For nonvector use only)
 */


#if defined(__SSE2__)
	#define VECTOR_SEL_FLOAT(a,b,c) _mm_or_ps(_mm_andnot_ps((__m128)c,a), _mm_and_ps((__m128)c,b))
	#define VECTOR_SEL_SINT(a,b,c) _mm_or_si128( _mm_and_si128(c,b),_mm_andnot_si128(c,a))

	#define VECTOR_SEL_UINT(a,b,c) _mm_or_si128( _mm_and_si128(c,b),_mm_andnot_si128(c,a))
	#define VECTOR_SEL_CHAR(a,b,c) _mm_or_si128( _mm_and_si128(c,b),_mm_andnot_si128(c,a))

	#define VECTOR_CVT_FLOAT_SINT(a) _mm_cvttps_epi32(a)
	#define VECTOR_CVT_SINT_FLOAT(a) _mm_cvtepi32_ps(a)
	#define VECTOR_CVT_FLOAT_UINT(a) _mm_cvttps_epi32(a)	
#elif defined(__VEC__)
	#define VECTOR_SEL_FLOAT(a,b,c) vec_sel(a,b,c)
	#define VECTOR_SEL_SINT(a,b,c) vec_sel(a,b,c)

	#define VECTOR_SEL_UINT(a,b,c) vec_sel(a,b,c)
	#define VECTOR_SEL_CHAR(a,b,c) vec_sel(a,b,c)

	#define VECTOR_CVT_FLOAT_SINT(a) vec_cts(a,0)
	#define VECTOR_CVT_SINT_FLOAT(a) vec_ctf(a,0)
	#define VECTOR_CVT_FLOAT_UINT(a) vec_ctu(a,0)
#else
	#define VECTOR_SEL_FLOAT(a,b,c) vector_sel(a,b,c)
	#define VECTOR_SEL_SINT(a,b,c) vector_sel(a,b,c)

	#define VECTOR_SEL_UINT(a,b,c) vector_sel(a,b,c)
	#define VECTOR_SEL_CHAR(a,b,c) vector_sel(a,b,c)

	#define VECTOR_CVT_FLOAT_SINT(a) vector_cvt_float_sint(a)
	#define VECTOR_CVT_SINT_FLOAT(a) vector_cvt_sint_float(a)
	#define VECTOR_CVT_FLOAT_UINT(a) vector_cvt_float_uint(a)

	#define VECTOR_SRAI_SINT(a,sh) vector_srai_sint(a,sh)
	#define VECTOR_SLI_SINT(a,sh) vector_sli_sint(a,sh)
	#define VECTOR_SLI_UINT(a,sh) vector_sli_sint(a,sh)
#endif

#if defined(__SSE2__)
#elif defined(__VEC__)
#else
	#include <nonvector/vector_sel.h>
	#include <nonvector/vector_cvt.h>
	#include <nonvector/vector_shifts.h>
#endif

#endif /* VECTORMISC_H_ */
