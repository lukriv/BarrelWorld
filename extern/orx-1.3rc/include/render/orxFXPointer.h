/* Orx - Portable Game Engine
 *
 * Copyright (c) 2008-2011 Orx-Project
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */

/**
 * @file orxFXPointer.h
 * @date 30/06/2008
 * @author iarwain@orx-project.org
 *
 * @todo
 */

/**
 * @addtogroup orxFXPointer
 *
 * FXPointer module
 * Allows to creates rendering special effects containers for objects.
 *
 * @{
 */


#ifndef _orxFXPOINTER_H_
#define _orxFXPOINTER_H_


#include "orxInclude.h"
#include "render/orxFX.h"
#include "object/orxStructure.h"


/** Misc defines
 */
#define orxFXPOINTER_KU32_FX_NUMBER             4


/** Internal FXPointer structure */
typedef struct __orxFXPOINTER_t                 orxFXPOINTER;


/** FXPointer module setup
 */
extern orxDLLAPI void orxFASTCALL               orxFXPointer_Setup();

/** Inits the FXPointer module
 * @return orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_Init();

/** Exits from the FXPointer module
 */
extern orxDLLAPI void orxFASTCALL               orxFXPointer_Exit();


/** Creates an empty FXPointer
 * @param[in]   _pstOwner       FXPointer's owner used for event callbacks (usually an orxOBJECT)
 * @return orxFXPOINTER / orxNULL
 */
extern orxDLLAPI orxFXPOINTER *orxFASTCALL      orxFXPointer_Create(const orxSTRUCTURE *_pstOwner);

/** Deletes an FXPointer
 * @param[in] _pstFXPointer     Concerned FXPointer
 * @return orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_Delete(orxFXPOINTER *_pstFXPointer);

/** Gets an FXPointer owner
 * @param[in]   _pstFXPointer   Concerned FXPointer
 * @return      orxSTRUCTURE / orxNULL
 */
extern orxDLLAPI orxSTRUCTURE *orxFASTCALL      orxFXPointer_GetOwner(const orxFXPOINTER *_pstFXPointer);

/** Enables/disables an FXPointer
 * @param[in]   _pstFXPointer   Concerned FXPointer
 * @param[in]   _bEnable        Enable / disable
 */
extern orxDLLAPI void orxFASTCALL               orxFXPointer_Enable(orxFXPOINTER *_pstFXPointer, orxBOOL _bEnable);

/** Is FXPointer enabled?
 * @param[in]   _pstFXPointer   Concerned FXPointer
 * @return      orxTRUE if enabled, orxFALSE otherwise
 */
extern orxDLLAPI orxBOOL orxFASTCALL            orxFXPointer_IsEnabled(const orxFXPOINTER *_pstFXPointer);


/** Adds an FX
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _pstFX        FX to add
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_AddFX(orxFXPOINTER *_pstFXPointer, orxFX *_pstFX);

/** Adds a delayed FX
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _pstFX        FX to add
 * @param[in]   _fDelay       Delay time
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_AddDelayedFX(orxFXPOINTER *_pstFXPointer, orxFX *_pstFX, orxFLOAT _fDelay);

/** Removes an FX
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _pstFX        FX to remove
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_RemoveFX(orxFXPOINTER *_pstFXPointer, orxFX *_pstFX);

/** Adds an FX using its config ID
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _zFXConfigID  Config ID of the FX to add
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_AddFXFromConfig(orxFXPOINTER *_pstFXPointer, const orxSTRING _zFXConfigID);

/** Adds a unique FX using its config ID
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _zFXConfigID  Config ID of the FX to add
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_AddUniqueFXFromConfig(orxFXPOINTER *_pstFXPointer, const orxSTRING _zFXConfigID);

/** Adds a delayed FX using its config ID
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _zFXConfigID  Config ID of the FX to add
 * @param[in]   _fDelay       Delay time
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_AddDelayedFXFromConfig(orxFXPOINTER *_pstFXPointer, const orxSTRING _zFXConfigID, orxFLOAT _fDelay);

/** Adds a unique delayed FX using its config ID
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _zFXConfigID  Config ID of the FX to add
 * @param[in]   _fDelay       Delay time
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_AddUniqueDelayedFXFromConfig(orxFXPOINTER *_pstFXPointer, const orxSTRING _zFXConfigID, orxFLOAT _fDelay);

/** Removes an FX using using its config ID
 * @param[in]   _pstFXPointer Concerned FXPointer
 * @param[in]   _zFXConfigID  Config ID of the FX to remove
 * @return      orxSTATUS_SUCCESS / orxSTATUS_FAILURE
 */
extern orxDLLAPI orxSTATUS orxFASTCALL          orxFXPointer_RemoveFXFromConfig(orxFXPOINTER *_pstFXPointer, const orxSTRING _zFXConfigID);

#endif /* _orxFXPointer_H_ */

/** @} */
