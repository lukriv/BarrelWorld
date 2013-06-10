/*
 * vectorlib.h
 *
 *  Created on: 12.1.2010
 *      Author: Luk�
 */

/*
 * This "library" provides abstract structure over vector intrinsics.
 * In this version are implemented functions only for SSE2. In future it will be provided abstract for PPC64 altivec intrinsics too.
 *
 * Code is intend for C++ compilers.
 *
 */

#ifndef VECTORLIB_H_
#define VECTORLIB_H_



#include <vectorstruct.h>

#include <vectorloadf.h>
#include <vectorloadsi.h>
#include <vectorloadui.h>
#include <vectorloadc.h>

#include <vectorlogicf.h>
#include <vectorlogicsi.h>
#include <vectorlogicui.h>
#include <vectorlogicc.h>

#include <vectorcmpf.h>
#include <vectorcmpsi.h>
#include <vectorcmpui.h>
#include <vectorcmpc.h>

#include <vectoraritf.h>
#include <vectoraritsi.h>
#include <vectoraritui.h>
#include <vectoraritc.h>

#include <vectormisc.h>

#endif /* VECTORLIB_H_ */
