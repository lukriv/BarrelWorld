/*
 * load_vector.h
 *
 *  Created on: 8.2.2010
 *      Author: Luk� K�ivsk�
 */

#ifndef LOAD_VECTOR_H_
#define LOAD_VECTOR_H_

#include "vector_struct.h"

//! Loads vector from float array
/*!
 * Minimal size of input array is 4.
 *
 * \param in Array of float
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (float* in)
{
	vector_t vec;

	vec.vf[0] = in[0];
	vec.vf[1] = in[1];
	vec.vf[2] = in[2];
	vec.vf[3] = in[3];

	return vec;
}

//! Loads vector from integer array
/*!
 * Minimal size of input array is 4.
 *
 * \param in Array of integer
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (int* in)
{
	vector_t vec;

	vec.vsi[0] = in[0];
	vec.vsi[1] = in[1];
	vec.vsi[2] = in[2];
	vec.vsi[3] = in[3];

	return vec;
}

//! Loads vector from unsigned integer array
/*!
 * Minimal size of input array is 4.
 *
 * \param in Array of unsigned integer
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (unsigned int* in)
{
	vector_t vec;

	vec.vui[0] = in[0];
	vec.vui[1] = in[1];
	vec.vui[2] = in[2];
	vec.vui[3] = in[3];

	return vec;
}


//! Loads vector from unsigned char array
/*!
 * Minimal size of input array is 16.
 *
 * \param in Array of unsigned char
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector ( char* in)
{
	vector_t vec;

	vec.vchar[0] = in[0];
	vec.vchar[1] = in[1];
	vec.vchar[2] = in[2];
	vec.vchar[3] = in[3];

	vec.vchar[4] = in[4];
	vec.vchar[5] = in[5];
	vec.vchar[6] = in[6];
	vec.vchar[7] = in[7];

	vec.vchar[8] = in[8];
	vec.vchar[9] = in[9];
	vec.vchar[10] = in[10];
	vec.vchar[11] = in[11];

	vec.vchar[12] = in[12];
	vec.vchar[13] = in[13];
	vec.vchar[14] = in[14];
	vec.vchar[15] = in[15];

	return vec;
}

//! Loads vector from four float parameters
/*!
 *
 * \param p0 input float parameter
 * \param p1 input float parameter
 * \param p2 input float parameter
 * \param p3 input float parameter
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (float p0, float p1, float p2, float p3)
{
	vector_t vec;

	vec.vf[0] = p0;
	vec.vf[1] = p1;
	vec.vf[2] = p2;
	vec.vf[3] = p3;

	return vec;
}


//! Loads vector from four integer parameters
/*!
 *
 * \param p0 input integer parameter
 * \param p1 input integer parameter
 * \param p2 input integer parameter
 * \param p3 input integer parameter
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (int p0, int p1, int p2, int p3)
{
	vector_t vec;

	vec.vsi[0] = p0;
	vec.vsi[1] = p1;
	vec.vsi[2] = p2;
	vec.vsi[3] = p3;

	return vec;
}


//! Loads vector from four unsigned integer parameters
/*!
 *
 * \param p0 input unsigned integer parameter
 * \param p1 input unsigned integer parameter
 * \param p2 input unsigned integer parameter
 * \param p3 input unsigned integer parameter
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3)
{
	vector_t vec;

	vec.vui[0] = p0;
	vec.vui[1] = p1;
	vec.vui[2] = p2;
	vec.vui[3] = p3;

	return vec;
}

//! Loads vector from sixteen char parameters
/*!
 *
 * \param p0 input char parameter
 * \param p1 input char parameter
 * \param p2 input char parameter
 * \param p3 input char parameter
 *
 * \param p4 input char parameter
 * \param p5 input char parameter
 * \param p6 input char parameter
 * \param p7 input char parameter
 *
 * \param p8 input char parameter
 * \param p9 input char parameter
 * \param p10 input char parameter
 * \param p11 input char parameter
 *
 * \param p12 input char parameter
 * \param p13 input char parameter
 * \param p14 input char parameter
 * \param p15 input char parameter
 *
 * \return Returns vector_t type.
 */
__inline vector_t load_vector (char p0, char p1, char p2, char p3,
		char p4, char p5, char p6, char p7,
		char p8, char p9, char p10, char p11,
		char p12, char p13, char p14, char p15)
{
	vector_t vec;

	vec.vchar[0] = p0;
	vec.vchar[1] = p1;
	vec.vchar[2] = p2;
	vec.vchar[3] = p3;

	vec.vchar[4] = p4;
	vec.vchar[5] = p5;
	vec.vchar[6] = p6;
	vec.vchar[7] = p7;

	vec.vchar[8] = p8;
	vec.vchar[9] = p9;
	vec.vchar[10] = p10;
	vec.vchar[11] = p11;

	vec.vchar[12] = p12;
	vec.vchar[13] = p13;
	vec.vchar[14] = p14;
	vec.vchar[15] = p15;

	return vec;
}

#endif /* LOAD_VECTOR_H_ */
