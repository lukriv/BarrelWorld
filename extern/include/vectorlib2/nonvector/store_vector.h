/*
 * store_vector.h
 *
 *  Created on: 8.2.2010
 *      Author: Lukáš Křivský
 */

#ifndef STORE_VECTOR_H_
#define STORE_VECTOR_H_


__inline void store_vector (float *out, vector_t vec)
{
	out[0] = vec.vf[0];
	out[1] = vec.vf[1];
	out[2] = vec.vf[2];
	out[3] = vec.vf[3];
}

__inline void store_vector (int *out, vector_t vec)
{
	out[0] = vec.vsi[0];
	out[1] = vec.vsi[1];
	out[2] = vec.vsi[2];
	out[3] = vec.vsi[3];
}

__inline void store_vector (unsigned int *out, vector_t vec)
{
	out[0] = vec.vui[0];
	out[1] = vec.vui[1];
	out[2] = vec.vui[2];
	out[3] = vec.vui[3];
}

__inline void store_vector (char *out, vector_t vec)
{
	out[0] = vec.vchar[0];
	out[1] = vec.vchar[1];
	out[2] = vec.vchar[2];
	out[3] = vec.vchar[3];

	out[4] = vec.vchar[4];
	out[5] = vec.vchar[5];
	out[6] = vec.vchar[6];
	out[7] = vec.vchar[7];

	out[8] = vec.vchar[8];
	out[9] = vec.vchar[9];
	out[10] = vec.vchar[10];
	out[11] = vec.vchar[11];

	out[12] = vec.vchar[12];
	out[13] = vec.vchar[13];
	out[14] = vec.vchar[14];
	out[15] = vec.vchar[15];
}

#endif /* STORE_VECTOR_H_ */
