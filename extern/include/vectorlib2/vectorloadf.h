/*
 * vectorload.h
 *
 *  Created on: 12.1.2010
 *      Author: Lukáš
 */

#ifndef VECTORLOADF_H_
#define VECTORLOADF_H_

#include <vectorstruct.h>

/*! \def LOAD_VECTOR_ALIGNED_FLOAT(in)
 *  \brief Loads vector from float array \a in.
 *
 *  Minimal size of input array is 4. Address must be 16-byte aligned.
 *  Parameter \a in has \a float* type.
 *  Return value is VECTOR_FLOAT type.
 *
 */

/*! \def LOAD_VECTOR_UNALIGNED_FLOAT(in)
 * \brief Loads vector from float array \a in.
 *
 * Minimal size of input array is 4. Address is not 16-byte aligned.
 * Parameter \a in has \a float* type.
 * Return value is VECTOR_FLOAT type.
 */

/*! \def LOAD_VECTOR_FLOAT4(in0,in1,in2,in3)
 * \brief Loads vector from four float parameters.
 *
 * Return value is VECTOR_FLOAT type.
 */

/*! \def LOAD_VECTOR_UNALIGNED_FLOAT(in)
 * \brief Loads vector from float array \a in.
 *
 * Minimal size of input array is 4. Address is not 16-byte aligned.
 * Parameter \a in has \a float* type.
 * Return value is VECTOR_FLOAT type.
 */

/*! \def STORE_VECTOR_ALIGNED_FLOAT(out,vec)
 * \brief Stores four vector elements into array.
 *
 * \a out is poiter (float*) to output array with minimal size 16 byte. Address of output array must be 16-byte aligned.
 * \a vec is vector to store.
 * No value is returned.
 */

/*! \def STORE_VECTOR_UNALIGNED_FLOAT(out,vec)
 * \brief Stores four vector elements into array.
 *
 * \a out is poiter (float*) to output array with minimal size 16 byte.
 * \a vec is vector to store.
 * No value is returned.
 */

/*! \def VECTOR_EXTRACT_FLOAT(vec,elem)
 * \brief Extract elem (int) from vectro (VECTOR_FLOAT)
 *
 * \a vec is input vector, \a elem is element to extract from vector.
 * It returns float value.
 */

/*! \def VECTOR_INSERT_FLOAT(scal,vec,elem)
 * \brief Insert scalar(float) into vector(VECTOR_FLOAT) to selected element(int)
 *
 * \a scal is input scalar value, \a vec is input vector, \a elem is selected element from vector which
 * will be replaced.
 * It returns new VECTOR_FLOAT value.
 */

/*! \def VECTOR_SPLATS_FLOAT(scal)
 * \brief Splat Scalar to Vector
 *
 * The single scalar value is replicated across all elements of a vector of the same type.
 * Parameter is scalar float value.
 * Return value is VECTOR_FLOAT type.
 */

/*! \def VECTOR_SPLAT_FLOAT(vec,elem)
 * \brief Vector Splat
 *
 * Each element of the result is component b of a.
 * Parameters are: \a vec is VECTOR_FLOAT and \a elem is int type.
 * Return value is VECTOR_FLOAT type.
 */


/*! \def VECTOR_ZERO_CONST_FLOAT
 *	\brief Vector Zero Constant
 * 
 *	Turn to zero vector constant.
 */


#if defined(__SSE2__)
	#define LOAD_VECTOR_ALIGNED_FLOAT(in) _mm_load_ps(in)
	#define LOAD_VECTOR_UNALIGNED_FLOAT(in) _mm_loadu_ps(in)
	#define LOAD_VECTOR_FLOAT4(in0,in1,in2,in3) _mm_setr_ps(in0,in1,in2,in3)
	#define STORE_VECTOR_ALIGNED_FLOAT(out,vec) _mm_store_ps(out, vec)
	#define STORE_VECTOR_UNALIGNED_FLOAT(out,vec) _mm_storeu_ps(out, vec)
	#define VECTOR_EXTRACT_FLOAT(vec,elem) vector_extract_float(vec,elem)
	#define VECTOR_INSERT_FLOAT(scal,vec,elem) vector_insert(scal,vec,elem)
	#define VECTOR_SPLATS_FLOAT(scal) _mm_set1_ps(scal)
	#define VECTOR_SPLAT_FLOAT(vec,elem) vector_sse_splat_float(vec,elem)
	#define VECTOR_ZERO_CONST_FLOAT	 ((__m128) {0.0f,0.0f,0.0f,0.0f})
#elif defined(__VEC__)
	#define LOAD_VECTOR_ALIGNED_FLOAT(in) vec_ld(0,in)
	#define LOAD_VECTOR_UNALIGNED_FLOAT(in) (vector float) _load_vec_unaligned((unsigned char*)in)
	#define LOAD_VECTOR_FLOAT4(in0,in1,in2,in3) _load_vec_float4(in0,in1,in2,in3)
	#define STORE_VECTOR_ALIGNED_FLOAT(out,vec) vec_st(vec,0,out)
	#define STORE_VECTOR_UNALIGNED_FLOAT(out,vec) _store_vec_unaligned((unsigned char*) out, (vector unsigned char) vec)
	#define VECTOR_EXTRACT_FLOAT(vec,elem) vec_extract(vec,elem)
	#define VECTOR_INSERT_FLOAT(scal,vec,elem) vec_insert(scal,vec,elem)
	#define VECTOR_SPLATS_FLOAT(scal) vec_splats(scal)
	#define VECTOR_SPLAT_FLOAT(vec,elem) vec_splat(vec,elem)
	#define VECTOR_ZERO_CONST_FLOAT	 ((vector float) {0.0f,0.0f,0.0f,0.0f})
#else
	#define LOAD_VECTOR_ALIGNED_FLOAT(in) load_vector(in)
	#define LOAD_VECTOR_UNALIGNED_FLOAT(in) load_vector(in)
	#define LOAD_VECTOR_FLOAT4(in0,in1,in2,in3) load_vector(in0,in1,in2,in3)
	#define STORE_VECTOR_ALIGNED_FLOAT(out,vec) store_vector(out,vec)
	#define STORE_VECTOR_UNALIGNED_FLOAT(out,vec) store_vector(out,vec)
	#define VECTOR_EXTRACT_FLOAT(vec,elem) vector_extract_float(vec,elem)
	#define VECTOR_INSERT_FLOAT(scal,vec,elem) vector_insert(scal,vec,elem)
	#define VECTOR_SPLATS_FLOAT(scal) vector_splats(scal)
	#define VECTOR_SPLAT_FLOAT(vec,elem) vector_splat_float(vec,elem)
	#define VECTOR_ZERO_CONST_FLOAT	 ((vector_t) {{0.0f,0.0f,0.0f,0.0f}})
#endif



#if defined(__SSE2__)
#include <sse/vector_extract.h>
#include <sse/vector_insert.h>
#include <sse/vector_sse_splat.h>

#elif defined(__VEC__)

#include "altivec/load_vec_unaligned.h"
#include "altivec/load_vec_float4.h"
#include "altivec/store_vec_unaligned.h"

#else

#include <nonvector/load_vector.h>
#include <nonvector/store_vector.h>
#include <nonvector/vector_extract.h>
#include <nonvector/vector_insert.h>
#include <nonvector/vector_splats.h>
#include <nonvector/vector_splat.h>

#endif




#endif /* VECTORLOADF_H_ */
