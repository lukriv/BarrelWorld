/*
 * sqrtf_veclib.h
 *
 *  Created on: 16.4.2010
 *      Author: Lukáš
 */

#ifndef SQRTF_VEC_H_
#define SQRTF_VEC_H_

#include <altivec.h>

/*
 * FUNCTION
 *	vector float _sqrtf4(vector float x)
 *
 * DESCRIPTION
 *	The _sqrtf4 computes the square root of the vector input "in"
 *	and returns the result.
 *
 *	The VMX implementation computes the square root by noting
 *      that sqrtf(x) = x / sqrtf(x). However, it does not produce
 * 	IEEE accuracy.
 *
 */
static __inline vector float _sqrtf4_vec(vector float in)
{

  vector float out;

  vector float y0;
  vector float zero = ((vector float) { 0.0f, 0.0f, 0.0f, 0.0f });

  /* Perform one iteration of the Newton-Raphsom method in single precision
   * arithmetic.
   */
  y0 = vec_rsqrte(in);
  out = vec_madd(vec_nmsub(in, vec_madd(y0, y0, zero), (vector float)(((vector unsigned int) { 0x40400001, 0x40400001, 0x40400001, 0x40400001 }))),
		 vec_madd(y0, vec_madd(in, ((vector float) { 0.5f, 0.5f, 0.5f, 0.5f }), zero), zero),
		 zero);
  out = vec_and(out, (vector float)vec_cmpgt(in, (vector float)(((vector unsigned int) { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF }))));
  return (out);

}


#endif /* SQRTF_VEC_H_ */
