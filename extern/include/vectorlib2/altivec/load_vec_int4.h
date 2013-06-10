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
#ifndef _LOAD_VEC_INT4_H_
#define _LOAD_VEC_INT4_H_	1

#include <altivec.h>

/*
 * FUNCTION
 * 	vector signed int _load_vec_int4(signed int i1, signed int i2,
 *					 signed int i3, signed int i4)
 *
 * DESCRIPTION
 * 	_load_vec_int4 loads 4 independent integer values (i1, i2, i3,
 *	i4) into a 128-bit integer vector and returns the vector.
 *	The vector is filled as follows:
 *           __________________________________
 *          |___i1___|___i2___|___i3___|___i4__|
 *          msb                              lsb
 */

static __inline vector signed int _load_vec_int4(signed int i1, signed int i2, signed int i3, signed int i4)
{
  vector signed int result;
  union {
    vector signed int iv;
    signed int i[4];
  } iv1, iv2, iv3, iv4;
  vector signed int i1i30000, i2i40000;

  iv1.i[0] = i1;
  iv3.i[0] = i3;
  iv2.i[0] = i2;
  iv4.i[0] = i4;

  i1i30000 = vec_mergeh(iv1.iv, iv3.iv);
  i2i40000 = vec_mergeh(iv2.iv, iv4.iv);

  result = vec_mergeh(i1i30000, i2i40000);

  return (result);
}

#endif /* _LOAD_VEC_INT4_H_ */
