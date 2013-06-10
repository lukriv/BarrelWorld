/* --------------------------------------------------------------  */
/* (C)Copyright 2001,2007,                                         */
/* International Business Machines Corporation,                    */
/* Sony Computer Entertainment, Incorporated,                      */
/* Toshiba Corporation,                                            */
/*                                                                 */
/* All Rights Reserved.                                            */
/*                                                                 */
/* Redistribution and use in source and binary forms, with or      */
/* without modification, are permitted provided that the           */
/* following conditions are met:                                   */
/*                                                                 */
/* - Redistributions of source code must retain the above copyright*/
/*   notice, this list of conditions and the following disclaimer. */
/*                                                                 */
/* - Redistributions in binary form must reproduce the above       */
/*   copyright notice, this list of conditions and the following   */
/*   disclaimer in the documentation and/or other materials        */
/*   provided with the distribution.                               */
/*                                                                 */
/* - Neither the name of IBM Corporation nor the names of its      */
/*   contributors may be used to endorse or promote products       */
/*   derived from this software without specific prior written     */
/*   permission.                                                   */
/*                                                                 */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND          */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,     */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE        */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR            */
/* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT    */
/* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;    */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN       */
/* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR    */
/* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */
#ifndef _LOAD_VEC_FLOAT4_H_
#define _LOAD_VEC_FLOAT4_H_	1


#include <altivec.h>

/*
 * FUNCTION
 * 	vector float _load_vec_float4(float f1, float f2, float f3, float f4)
 *
 * DESCRIPTION
 * 	_load_vec_float4 loads 4 independent floating-point values (f1, f2, f3,
 *	f4) into a 128-bit floating point vector and returns the vector.
 *	The vector is filled as follows:
 *           __________________________________
 *          |___f1___|___f2___|___f3___|___f4__|
 *          msb                              lsb
 */

static __inline vector float _load_vec_float4(float f1, float f2, float f3, float f4)
{
  vector float result;
  union {
    vector float fv;
    float f[4];
  } fv1, fv2, fv3, fv4;
  vector float f1f30000, f2f40000;

  fv1.f[0] = f1;
  fv3.f[0] = f3;
  fv2.f[0] = f2;
  fv4.f[0] = f4;

  f2f40000 = vec_mergeh(fv2.fv, fv4.fv);
  f1f30000 = vec_mergeh(fv1.fv, fv3.fv);

  result = vec_mergeh(f1f30000, f2f40000);

  return (result);
}


#endif /* _LOAD_VEC_FLOAT4_H_ */

