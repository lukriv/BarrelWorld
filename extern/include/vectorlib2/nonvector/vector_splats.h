/*
 * vector_splats.h
 *
 *  Created on: 8.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef VECTOR_SPLATS_H_
#define VECTOR_SPLATS_H_

#include <stdint.h>
#include "vector_struct.h"

//! Splat Scalar to Vector
/*!
 *
 * The single scalar value is replicated across all elements of a vector of the same type and the result is returned
 *
 * \param scalar Input scalar parameter.
 *
 * \return Vector with four same values.
 */
__inline vector_t vector_splats (float scalar)
{
	vector_t vec;

	vec.vf[0] = scalar;
	vec.vf[1] = scalar;
	vec.vf[2] = scalar;
	vec.vf[3] = scalar;

	return vec;

}

//! Splat Scalar to Vector
/*!
 *
 * The single scalar value is replicated across all elements of a vector of the same type and the result is returned
 *
 * \param scalar Input scalar parameter.
 * \return Vector with four same values.
 */
__inline vector_t vector_splats (int scalar)
{
	vector_t vec;

	vec.vsi[0] = scalar;
	vec.vsi[1] = scalar;
	vec.vsi[2] = scalar;
	vec.vsi[3] = scalar;

	return vec;

}

//! Splat Scalar to Vector
/*!
 *
 * The single scalar value is replicated across all elements of a vector of the same type and the result is returned
 *
 * \param scalar Input scalar parameter.
 * \return Vector with four same values.
 */
__inline vector_t vector_splats (unsigned int scalar)
{
	vector_t vec;

	vec.vui[0] = scalar;
	vec.vui[1] = scalar;
	vec.vui[2] = scalar;
	vec.vui[3] = scalar;

	return vec;

}

//! Splat Scalar to Vector
/*!
 *
 * The single scalar value is replicated across all elements of a vector of the same type and the result is returned
 *
 * \param scalar Input scalar parameter.
 * \return Type vector_t.
 */
__inline vector_t vector_splats ( char scalar)
{
	vector_t vec;

	vec.vchar[0] = scalar;
	vec.vchar[1] = scalar;
	vec.vchar[2] = scalar;
	vec.vchar[3] = scalar;

	vec.vchar[4] = scalar;
	vec.vchar[5] = scalar;
	vec.vchar[6] = scalar;
	vec.vchar[7] = scalar;

	vec.vchar[8] = scalar;
	vec.vchar[9] = scalar;
	vec.vchar[10] = scalar;
	vec.vchar[11] = scalar;

	vec.vchar[12] = scalar;
	vec.vchar[13] = scalar;
	vec.vchar[14] = scalar;
	vec.vchar[15] = scalar;

	return vec;

}

#endif /* VECTOR_SPLATS_H_ */
