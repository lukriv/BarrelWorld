/*
 * vectorstruct.h
 *
 *  Created on: 12.1.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTORSTRUCT_H_
#define VECTORSTRUCT_H_



#if defined(__SSE2__)
	#define VECTOR_FLOAT __m128
	#define VECTOR_SINT __m128i
	#define VECTOR_UINT __m128i
	#define VECTOR_CHAR __m128i

	#include <emmintrin.h>

#elif defined(__VEC__)
	#define VECTOR_FLOAT vector float
	#define VECTOR_SINT vector signed int
	#define VECTOR_UINT vector unsigned int
	#define VECTOR_CHAR vector char

	#include <altivec.h>

#else
	#define VECTOR_FLOAT vector_t
	#define VECTOR_SINT vector_t
	#define VECTOR_UINT vector_t
	#define VECTOR_CHAR vector_t

	#include <nonvector/vector_struct.h>

#endif

//vector_int32;



#endif /* VECTORSTRUCT_H_ */
