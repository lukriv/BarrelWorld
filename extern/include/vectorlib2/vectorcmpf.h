/*
 * vectorcmpf.h
 *
 *  Created on: 10.3.2010
 *      Author: Lukáš
 */

#ifndef VECTORCMPF_H_
#define VECTORCMPF_H_



// int vector_any_eq (vector_float a, vector_float b);
// int vector_any_lt (vector_float a, vector_float b);
// int vector_any_le (vector_float a, vector_float b);
// int vector_any_gt (vector_float a, vector_float b);
// int vector_any_ge (vector_float a, vector_float b);

// int vector_all_eq (vector_float a, vector_float b);
// int vector_all_lt (vector_float a, vector_float b);
// int vector_all_le (vector_float a, vector_float b);
// int vector_all_gt (vector_float a, vector_float b);
// int vector_all_ge (vector_float a, vector_float b);

/*! \def VECTOR_CMPEQ_FLOAT(a,b)
 *
 *	\brief Vector Compare Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is VECTOR_UINT type.
 *  Each element of the result is all ones if the corresponding element of a is equal to the
 *  corresponding element of b. Otherwise, it returns all zeros.
 *
 */

/*! \def VECTOR_CMPLT_FLOAT(a,b)
 *
 *	\brief Vector Compare Less Than
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is VECTOR_UINT type.
 *  Each element of the result is all ones if the corresponding element of a is less than the
 *   corresponding element of b. Otherwise, it returns all zeros.
 *
 */

/*! \def VECTOR_CMPLE_FLOAT(a,b)
 *
 *	\brief Vector Compare Less Than or Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is VECTOR_UINT type.
 *  Each element of the result is all ones if the corresponding element of a is less than or equal
 *  to the corresponding element of b. Otherwise, it returns all zeros.
 *
 */

/*! \def VECTOR_CMPGT_FLOAT(a,b)
 *
 *	\brief Vector Compare Greater Than
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is VECTOR_UINT type.
 *  Each element of the result is all ones if the corresponding element of a is greater than the
 *  corresponding element of b. Otherwise, it returns all zeros.
 *
 */

/*! \def VECTOR_CMPGE_FLOAT(a,b)
 *
 *	\brief Vector Compare Greater Than or Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is VECTOR_UINT type.
 *  Each element of the result is all ones if the corresponding element of a is greater than or
 *  equal to the corresponding element of b. Otherwise, it returns all zeros.
 *
 */

/*! \def VECTOR_ANY_EQ_FLOAT(a,b)
 *
 *	\brief Any Element Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_any_eq returns 1 if any element of a is equal to the corresponding
 *	element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ANY_NE_FLOAT(a,b)
 *
 *	\brief Any Element Not Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_any_eq returns 1 if any element of a is not equal to the corresponding
 *	element of b. Otherwise, it returns 0.
 *
 */


/*! \def VECTOR_ANY_LT_FLOAT(a,b)
 *
 *	\brief Any Element Less Than
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_any_lt returns 1 if any element of a is less than the corresponding
 *  element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ANY_LE_FLOAT(a,b)
 *
 *	\brief Any Element Less Than or Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_any_le returns 1 if any element of a is less than or equal to the
 *  corresponding element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ANY_GT_FLOAT(a,b)
 *
 *	\brief Any Element Greater Than
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_any_gt returns 1 if any element of a is greater than the corresponding
 *  element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ANY_GE_FLOAT(a,b)
 *
 *	\brief Any Element Greater Than or Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_any_ge returns 1 if any element of a is greater than or equal to the
 * 	corresponding element of b. Otherwise, it returns 0.
 *
 */


/*! \def VECTOR_ALL_EQ_FLOAT(a,b)
 *
 *	\brief All Elements Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_all_eq returns 1 if every element of a is equal to the corresponding
 * 	element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ALL_LT_FLOAT(a,b)
 *
 *	\brief All Elements Less Than
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_all_le returns 1 if every element of a is less than or equal to the
 * 	corresponding element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ALL_LE_FLOAT(a,b)
 *
 *	\brief All Elements Less Than or Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_all_le returns 1 if every element of a is less than or equal to the
 *  corresponding element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ALL_GT_FLOAT(a,b)
 *
 *
 *	\brief All Elements Greater Than
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_all_gt returns 1 if every element of a is greater than the corresponding
 *	element of b. Otherwise, it returns 0.
 *
 */

/*! \def VECTOR_ALL_GE_FLOAT(a,b)
 *
 *
 *	\brief All Elements Greater Than or Equal
 *
 *  Parameters \a a and \a b are VECTOR_FLOAT type. Return value is integer type.
 *  The predicate vec_all_ge returns 1 if every element of a is greater than or equal to the
 * 	corresponding element of b. Otherwise, it returns 0.
 *
 */

#if defined(__SSE2__)
	#define VECTOR_CMPEQ_FLOAT(a,b) (__m128i) _mm_cmpeq_ps(a,b)
	#define VECTOR_CMPLT_FLOAT(a,b) (__m128i) _mm_cmplt_ps(a,b)
	#define VECTOR_CMPLE_FLOAT(a,b) (__m128i) _mm_cmple_ps(a,b)
	#define VECTOR_CMPGT_FLOAT(a,b) (__m128i) _mm_cmpgt_ps(a,b)
	#define VECTOR_CMPGE_FLOAT(a,b) (__m128i) _mm_cmpge_ps(a,b)

	#define VECTOR_ANY_EQ_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) != 0)
	#define VECTOR_ANY_NE_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpneq_ps(a,b)) != 0)
	#define VECTOR_ANY_LT_FLOAT(a,b) (_mm_movemask_ps(_mm_cmplt_ps(a,b)) != 0)
	#define VECTOR_ANY_LE_FLOAT(a,b) (_mm_movemask_ps(_mm_cmple_ps(a,b)) != 0)
	#define VECTOR_ANY_GT_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpgt_ps(a,b)) != 0)
	#define VECTOR_ANY_GE_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpge_ps(a,b)) != 0)

	#define VECTOR_ALL_EQ_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) == 0xf)
	#define VECTOR_ALL_LT_FLOAT(a,b) (_mm_movemask_ps(_mm_cmplt_ps(a,b)) == 0xf)
	#define VECTOR_ALL_LE_FLOAT(a,b) (_mm_movemask_ps(_mm_cmple_ps(a,b)) == 0xf)
	#define VECTOR_ALL_GT_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpgt_ps(a,b)) == 0xf)
	#define VECTOR_ALL_GE_FLOAT(a,b) (_mm_movemask_ps(_mm_cmpge_ps(a,b)) == 0xf)

#elif defined(__VEC__)
	#define VECTOR_CMPEQ_FLOAT(a,b) (vector unsigned int) vec_cmpeq(a,b)
	#define VECTOR_CMPLT_FLOAT(a,b) (vector unsigned int) vec_cmplt(a,b)
	#define VECTOR_CMPLE_FLOAT(a,b) (vector unsigned int) vec_cmple(a,b)
	#define VECTOR_CMPGT_FLOAT(a,b) (vector unsigned int) vec_cmpgt(a,b)
	#define VECTOR_CMPGE_FLOAT(a,b) (vector unsigned int) vec_cmpge(a,b)

	#define VECTOR_ANY_EQ_FLOAT(a,b) vec_any_eq(a,b)
	#define VECTOR_ANY_NE_FLOAT(a,b) vec_any_ne(a,b)
	#define VECTOR_ANY_LT_FLOAT(a,b) vec_any_lt(a,b)
	#define VECTOR_ANY_LE_FLOAT(a,b) vec_any_le(a,b)
	#define VECTOR_ANY_GT_FLOAT(a,b) vec_any_gt(a,b)
	#define VECTOR_ANY_GE_FLOAT(a,b) vec_any_ge(a,b)

	#define VECTOR_ALL_EQ_FLOAT(a,b) vec_all_eq(a,b)
	#define VECTOR_ALL_LT_FLOAT(a,b) vec_all_lt(a,b)
	#define VECTOR_ALL_LE_FLOAT(a,b) vec_all_le(a,b)
	#define VECTOR_ALL_GT_FLOAT(a,b) vec_all_gt(a,b)
	#define VECTOR_ALL_GE_FLOAT(a,b) vec_all_ge(a,b)
#else
	#define VECTOR_CMPEQ_FLOAT(a,b) vector_cmpeq_float(a,b)
	#define VECTOR_CMPLT_FLOAT(a,b) vector_cmplt_float(a,b)
	#define VECTOR_CMPLE_FLOAT(a,b) vector_cmple_float(a,b)
	#define VECTOR_CMPGT_FLOAT(a,b) vector_cmpgt_float(a,b)
	#define VECTOR_CMPGE_FLOAT(a,b) vector_cmpge_float(a,b)

	#define VECTOR_ANY_EQ_FLOAT(a,b) vector_any_eq_float(a,b)
	#define VECTOR_ANY_NE_FLOAT(a,b) vector_any_ne_float(a,b)
	#define VECTOR_ANY_LT_FLOAT(a,b) vector_any_lt_float(a,b)
	#define VECTOR_ANY_LE_FLOAT(a,b) vector_any_le_float(a,b)
	#define VECTOR_ANY_GT_FLOAT(a,b) vector_any_gt_float(a,b)
	#define VECTOR_ANY_GE_FLOAT(a,b) vector_any_ge_float(a,b)

	#define VECTOR_ALL_EQ_FLOAT(a,b) vector_all_eq_float(a,b)
	#define VECTOR_ALL_LT_FLOAT(a,b) vector_all_lt_float(a,b)
	#define VECTOR_ALL_LE_FLOAT(a,b) vector_all_le_float(a,b)
	#define VECTOR_ALL_GT_FLOAT(a,b) vector_all_gt_float(a,b)
	#define VECTOR_ALL_GE_FLOAT(a,b) vector_all_ge_float(a,b)
#endif

#if defined(__SSE2__)
#elif defined(__VEC__)
#else
	#include "nonvector/vector_cmpeq.h"
	#include "nonvector/vector_cmplt.h"
	#include "nonvector/vector_cmple.h"
	#include "nonvector/vector_cmpgt.h"
	#include "nonvector/vector_cmpge.h"

	#include "nonvector/vector_any_eq.h"
	#include "nonvector/vector_any_ne.h"
	#include "nonvector/vector_any_lt.h"
	#include "nonvector/vector_any_le.h"
	#include "nonvector/vector_any_gt.h"
	#include "nonvector/vector_any_ge.h"

	#include "nonvector/vector_all_eq.h"
	#include "nonvector/vector_all_lt.h"
	#include "nonvector/vector_all_le.h"
	#include "nonvector/vector_all_gt.h"
	#include "nonvector/vector_all_ge.h"
#endif


#endif /* VECTORCMPF_H_ */
