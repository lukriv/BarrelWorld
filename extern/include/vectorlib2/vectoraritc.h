/*
 * vectoraritc.h
 *
 *  Created on: 17.4.2010
 *      Author: Lukáš
 */

#ifndef VECTORARITC_H_
#define VECTORARITC_H_

/*! \def VECTOR_ADD_CHAR(a,b)
 *
 *	\brief Vector Add
 *
 * Each element of a is added to the corresponding element of b. Each sum is placed in the
 * corresponding element of result vector.
 *
 */

/*! \def VECTOR_SUB_CHAR(a,b)
 *
 *	\brief Vector Subtract
 *
 * Each element of the result is the difference between the corresponding elements of a and b.
 *
 */


/*! \def VECTOR_MIN_CHAR(a,b)
 *
 *	\brief Vector Compare Equal
 *
 *  Each element of the result is the smaller of the corresponding elements of a and b.
 *
 */

/*! \def VECTOR_MAX_CHAR(a,b)
 *
 *	\brief Vector Compare Equal
 *
 *  Each element of the result is the larger of the corresponding elements of a and b.
 *
 */

#if defined(__SSE2__)
	#define VECTOR_ADD_CHAR(a,b) _mm_add_epi8(a,b)
	#define VECTOR_SUB_CHAR(a,b) _mm_sub_epi8(a,b)

	#define VECTOR_MIN_CHAR(a,b) _sse_min_char(a,b)
	#define VECTOR_MAX_CHAR(a,b) _sse_max_char(a,b)


#elif defined(__VEC__)
	#define VECTOR_ADD_CHAR(a,b) vec_add(a,b)
	#define VECTOR_SUB_CHAR(a,b) vec_sub(a,b)

	#define VECTOR_MIN_CHAR(a,b) vec_min(a,b)
	#define VECTOR_MAX_CHAR(a,b) vec_max(a,b)
#else
	#define VECTOR_ADD_CHAR(a,b) vector_add_char(a,b)
	#define VECTOR_SUB_CHAR(a,b) vector_sub_char(a,b)

	#define VECTOR_MIN_CHAR(a,b) vector_min_char(a,b)
	#define VECTOR_MAX_CHAR(a,b) vector_max_char(a,b)
#endif

#if defined(__SSE2__)
	#include <sse/vector_sse_max.h>
	#include <sse/vector_sse_min.h>
#elif defined(__VEC__)
#else
	#include <nonvector/vector_add.h>
	#include <nonvector/vector_sub.h>

	#include <nonvector/vector_min.h>
	#include <nonvector/vector_max.h>

#endif

#endif /* VECTORARITC_H_ */
