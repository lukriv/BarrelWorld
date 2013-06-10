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
#ifndef _STORE_VEC_UNALIGNED_H_
#define _STORE_VEC_UNALIGNED_H_

#include <altivec.h>

/*
 * FUNCTION
 *	void _store_vec_unaligned(unsigned char * ptr, vector unsigned char data)
 *
 * DESCRIPTION
 *	_store_vec_unaligned store a quadword (vector) to memory at the
 *      unalignd address specified by the parameter ptr. Data surrounding
 *	the quadword is unaffected.
 */

static __inline void _store_vec_unaligned(unsigned char * ptr, vector unsigned char data)
{
  vector unsigned char qw0, qw1;
  vector unsigned char mask;

  vector unsigned char pattern;

  qw0 = *((vector unsigned char *)(ptr));
  qw1 = *((vector unsigned char *)(ptr+16));

  pattern = vec_lvsr(0, ptr);

  mask = vec_perm(((vector unsigned char) {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}),
  			((vector unsigned char) {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}), pattern);
  data = vec_perm(data, data, pattern);

  qw0 = vec_sel(qw0, data, mask);
  qw1 = vec_sel(data, qw1, mask);

  *((vector unsigned char *)(ptr)) = qw0;
  *((vector unsigned char *)(ptr+16)) = qw1;
}

#endif /*  _STORE_VEC_UNALIGNED_H_ */



