/*
 * vectorloadi.h
 *
 *  Created on: 7.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTORLOADSI_H_
#define VECTORLOADSI_H_

#include <vectorstruct.h>

/*! \def LOAD_VECTOR_ALIGNED_SINT(in)
 *  \brief Loads vector from signed int array \a in.
 *
 *  Minimal size of input array is 4. Address must be 16-byte aligned.
 *  Parameter \a in has \a signed int* type.
 *  Return value is VECTOR_SINT type.
 *
 */

/*! \def LOAD_VECTOR_UNALIGNED_SINT(in)
 * \brief Loads vector from signed int array \a in.
 *
 * Minimal size of input array is 4. Address is not 16-byte aligned.
 * Parameter \a in has \a signed int* type.
 * Return value is VECTOR_SINT type.
 */

/*! \def LOAD_VECTOR_SINT4(in0,in1,in2,in3)
 * \brief Loads vector from four signed int parameters.
 *
 * Return value is VECTOR_SINT type.
 */

/*! \def LOAD_VECTOR_UNALIGNED_SINT(in)
 * \brief Loads vector from signed int array \a in.
 *
 * Minimal size of input array is 4. Address is not 16-byte aligned.
 * Parameter \a in has \a signed int* type.
 * Return value is VECTOR_SINT type.
 */

/*! \def STORE_VECTOR_ALIGNED_SINT(out,vec)
 * \brief Stores four vector elements into array.
 *
 * \a out is poiter (signed int*) to output array with minimal size 16 byte. Address of output array must be 16-byte aligned.
 * \a vec is vector to store.
 * No value is returned.
 */

/*! \def STORE_VECTOR_UNALIGNED_SINT(out,vec)
 * \brief Stores four vector elements into array.
 *
 * \a out is poiter (signed int*) to output array with minimal size 16 byte.
 * \a vec is vector to store.
 * No value is returned.
 */

/*! \def VECTOR_EXTRACT_SINT(vec,elem)
 * \brief Extract elem (int) from vectro (VECTOR_SINT)
 *
 * \a vec is input vector, \a elem is element to extract from vector.
 * It returns signed int value.
 */

/*! \def VECTOR_INSERT_SINT(scal,vec,elem)
 * \brief Insert scalar(signed int) into vector(VECTOR_SINT) to selected element(int)
 *
 * \a scal is input scalar value, \a vec is input vector, \a elem is selected element from vector which
 * will be replaced.
 * It returns new VECTOR_SINT value.
 */

/*! \def VECTOR_SPLATS_SINT(scal)
 * \brief Splat Scalar to Vector
 *
 * The single scalar value is replicated across all elements of a vector of the same type.
 * Parameter is scalar signed int value.
 * Return value is VECTOR_SINT type.
 */

/*! \def VECTOR_SPLAT_SINT(vec,elem)
 * \brief Vector Splat
 *
 * Each element of the result is component b of a.
 * Parameters are: \a vec is VECTOR_SINT and \a elem is int type.
 * Return value is VECTOR_SINT type.
 */

/*! \def VECTOR_ZERO_CONST_SINT
 *	\brief Vector Zero Constant
 * 
 *	Turn to zero vector constant.
 */


#if defined(__SSE2__)
	#define LOAD_VECTOR_ALIGNED_SINT(in) _mm_load_si128((__m128i*) in)
	#define LOAD_VECTOR_UNALIGNED_SINT(in) _mm_loadu_si128((__m128i*)in)
	#define LOAD_VECTOR_SINT4(in0,in1,in2,in3) _mm_setr_epi32(in0,in1,in2,in3)
	#define STORE_VECTOR_ALIGNED_SINT(out,vec) _mm_store_si128((__m128i*)out, vec)
	#define STORE_VECTOR_UNALIGNED_SINT(out,vec) _mm_storeu_si128((__m128i*)out, vec)
	#define VECTOR_EXTRACT_SINT(vec,elem) vector_extract_sint(vec,elem)
	#define VECTOR_INSERT_SINT(scal,vec,elem) vector_insert(scal,vec,elem)
	#define VECTOR_SPLATS_SINT(scal) _mm_set1_epi32(scal)
	#define VECTOR_SPLAT_SINT(vec,elem) vector_splat_sint(vec,elem)
	#define VECTOR_ZERO_CONST_SINT ((__m128i) {0})
#elif defined(__VEC__)
	#define LOAD_VECTOR_ALIGNED_SINT(in) vec_ld(0,in)
	#define LOAD_VECTOR_UNALIGNED_SINT(in) (vector signed int) _load_vec_unaligned((unsigned char*)in)
	#define LOAD_VECTOR_SINT4(in0,in1,in2,in3) _load_vec_int4(in0,in1,in2,in3)
	#define STORE_VECTOR_ALIGNED_SINT(out,vec) vec_st(vec,0,out)
	#define STORE_VECTOR_UNALIGNED_SINT(out,vec) _store_vec_unaligned((unsigned char*) out, (vector unsigned char) vec)
	#define VECTOR_EXTRACT_SINT(vec,elem) vec_extract(vec,elem)
	#define VECTOR_INSERT_SINT(scal,vec,elem) vec_insert(scal,vec,elem)
	#define VECTOR_SPLATS_SINT(scal) vec_splats(scal)
	#define VECTOR_SPLAT_SINT(vec,elem) vec_splat(vec,elem)
	#define VECTOR_ZERO_CONST_SINT ((vector signed int) {0,0,0,0})
#else
	#define LOAD_VECTOR_ALIGNED_SINT(in) load_vector(in)
	#define LOAD_VECTOR_UNALIGNED_SINT(in) load_vector(in)
	#define LOAD_VECTOR_SINT4(in0,in1,in2,in3) load_vector(in0,in1,in2,in3)
	#define STORE_VECTOR_ALIGNED_SINT(out,vec) store_vector(out,vec)
	#define STORE_VECTOR_UNALIGNED_SINT(out,vec) store_vector(out,vec)
	#define VECTOR_EXTRACT_SINT(vec,elem) vector_extract_sint(vec,elem)
	#define VECTOR_INSERT_SINT(scal,vec,elem) vector_insert(scal,vec,elem)
	#define VECTOR_SPLATS_SINT(scal) vector_splats(scal)
	#define VECTOR_SPLAT_SINT(vec,elem) vector_splat_sint(vec,elem)
	#define VECTOR_ZERO_CONST_SINT ((vector_t) {{0,0,0,0}})
#endif



#if defined(__SSE2__)
#include <sse/vector_extract.h>
#include <sse/vector_insert.h>
#include <sse/vector_sse_splat.h>

#elif defined(__VEC__)

#include "altivec/load_vec_unaligned.h"
#include "altivec/load_vec_int4.h"
#include "altivec/store_vec_unaligned.h"

#else

#include <nonvector/load_vector.h>
#include <nonvector/store_vector.h>
#include <nonvector/vector_extract.h>
#include <nonvector/vector_insert.h>
#include <nonvector/vector_splats.h>
#include <nonvector/vector_splat.h>

#endif

#endif /* VECTORLOADI_H_ */
