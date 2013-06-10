/*
 * vectoraritf.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef VECTORARITF_H_
#define VECTORARITF_H_

/*! \def VECTOR_ADD_FLOAT(a,b)
 *
 *	\brief Vector Add
 *
 * Each element of a is added to the corresponding element of b. Each sum is placed in the
 * corresponding element of result vector.
 *
 */

/*! \def VECTOR_SUB_FLOAT(a,b)
 *
 *	\brief Vector Subtract
 *
 * Each element of the result is the difference between the corresponding elements of a and b.
 *
 */

/*! \def VECTOR_MUL_FLOAT(a,b)
 *
 *	\brief Vector Multiply
 *
 * Each element of the result is the product of the corresponding elements of a and b.
 *
 */

/*! \def VECTOR_MUL_FLOAT3(a,b,zero)
 *
 *	\brief Vector Multiply
 *
 * Each element of the result is the product of the corresponding elements of a and b. 
 * Parameter \a zero is used only with altivec simd extension for zero element.
 *
 */

/*! \def VECTOR_MADD_FLOAT(a,b,c)
 *
 *	\brief Vector Multiply Add
 *
 * Each element of the result is the sum of the corresponding element of c and the product of
 * the corresponding elements of a and b.
 *
 */

/*! \def VECTOR_NMSUB_FLOAT(a,b,c)
 *
 *	\brief Vector Negative Multiply Subtract
 *
 * Each element of the result is the negative of the difference of the corresponding element of
 * c and the product of the corresponding elements of a and b.
 *
 */

/*! \def VECTOR_DIV_FLOAT(a,b)
 *
 *	\brief Vector Divide
 *
 * Divides the four single-precision, floating-point values of a and b.
 *
 */

/*! \def VECTOR_SQRT_FLOAT(a)
 *
 *	\brief Vector Square Roots
 *
 * Computes the square roots of the four single-precision, floating-point values of a.
 *
 */

/*! \def VECTOR_RE_FLOAT(a)
 *
 *	\brief Vector Reciprocal Estimate
 *
 * Each element of the result is an estimate of the reciprocal to the corresponding element
 * of a.
 *
 */

/*! \def VECTOR_RSQRT_FLOAT(a)
 *
 *	\brief Vector Reciprocal Square Root Estimate
 *
 * Each element of the result is an estimate of the reciprocal square root of the corresponding
 * element of a.
 *
 */

/*! \def VECTOR_MIN_FLOAT(a,b)
 *
 *	\brief Vector Compare Equal
 *
 *  Each element of the result is the smaller of the corresponding elements of a and b.
 *
 */

/*! \def VECTOR_MAX_FLOAT(a,b)
 *
 *	\brief Vector Compare Equal
 *
 *  Each element of the result is the larger of the corresponding elements of a and b.
 *
 */

#if defined(__SSE2__)
	#define VECTOR_ADD_FLOAT(a,b) _mm_add_ps(a,b)
	#define VECTOR_SUB_FLOAT(a,b) _mm_sub_ps(a,b)
	#define VECTOR_MUL_FLOAT(a,b) _mm_mul_ps(a,b)
	#define VECTOR_MUL_FLOAT3(a,b,zero) _mm_mul_ps(a,b)
	#define VECTOR_MADD_FLOAT(a,b,c) _mm_add_ps( _mm_mul_ps(a,b), c)
	#define VECTOR_NMSUB_FLOAT(a,b,c) _mm_sub_ps(c, _mm_mul_ps(a, b))
	#define VECTOR_DIV_FLOAT(a,b) _mm_div_ps(a,b)

	#define VECTOR_SQRT_FLOAT(a) _mm_sqrt_ps(a)
	#define VECTOR_RE_FLOAT(a) _mm_rcp_ps(a)
	#define VECTOR_RSQRT_FLOAT(a) _mm_rsqrt_ps(a)

	#define VECTOR_MIN_FLOAT(a,b) _mm_min_ps(a,b)
	#define VECTOR_MAX_FLOAT(a,b) _mm_max_ps(a,b)


#elif defined(__VEC__)
	#define VECTOR_ADD_FLOAT(a,b) vec_add(a,b)
	#define VECTOR_SUB_FLOAT(a,b) vec_sub(a,b)
	#define VECTOR_MUL_FLOAT(a,b) vec_madd(a, b, ((vector float) {0.0,0.0,0.0,0.0}) )
	#define VECTOR_MUL_FLOAT3(a,b,zero) vec_madd(a, b, zero)
	#define VECTOR_MADD_FLOAT(a,b,c) vec_madd(a,b,c)
	#define VECTOR_NMSUB_FLOAT(a,b,c) vec_nmsub(a,b,c)
	#define VECTOR_DIV_FLOAT(a,b) (a/b)

	#define VECTOR_SQRT_FLOAT(a) _sqrtf4_vec(a)
	#define VECTOR_RE_FLOAT(a) vec_re(a)
	#define VECTOR_RSQRT_FLOAT(a) vec_rsqrte(a)

	#define VECTOR_MIN_FLOAT(a,b) vec_min(a,b)
	#define VECTOR_MAX_FLOAT(a,b) vec_max(a,b)
#else
	#define VECTOR_ADD_FLOAT(a,b) vector_add_float(a,b)
	#define VECTOR_SUB_FLOAT(a,b) vector_sub_float(a,b)
	#define VECTOR_MUL_FLOAT(a,b,zero) vector_mul_float(a,b)
	#define VECTOR_MADD_FLOAT(a,b,c) vector_madd_float(a,b,c)
	#define VECTOR_NMSUB_FLOAT(a,b,c) vector_nmsub_float(a,b,c)
	#define VECTOR_DIV_FLOAT(a,b) vector_div_float(a,b)

	#define VECTOR_SQRT_FLOAT(a) vector_sqrt_float(a)
	#define VECTOR_RE_FLOAT(a) vector_re_float(a)
	#define VECTOR_RSQRT_FLOAT(a) vector_rsqrt_float(a)

	#define VECTOR_MIN_FLOAT(a,b) vector_min_float(a,b)
	#define VECTOR_MAX_FLOAT(a,b) vector_max_float(a,b)
#endif

#if defined(__SSE2__)
#elif defined(__VEC__)
	#include <altivec/sqrtf_vec.h>
#else
	#include <nonvector/vector_add.h>
	#include <nonvector/vector_sub.h>
	#include <nonvector/vector_mul.h>
	#include <nonvector/vector_madd.h>
	#include <nonvector/vector_nmsub.h>
	#include <nonvector/vector_div.h>

	#include <nonvector/vector_sqrt.h>
	#include <nonvector/vector_re.h>
	#include <nonvector/vector_rsqrt.h>

	#include <nonvector/vector_min.h>
	#include <nonvector/vector_max.h>

#endif


#endif /* VECTORARITF_H_ */
