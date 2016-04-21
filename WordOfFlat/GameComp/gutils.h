#ifndef __GAME_UTILITIES__H__
#define __GAME_UTILITIES__H__

#include <GameSystem/gerror.h>
#include "RenderComp/grendersystem.h"
#include "MenuComp/gmenumgr.h"
#include "transformComp/gtranscomp.h"
#include <bullet/LinearMath/btVector3.h>

class GameUtils
{
public:
	
	/**
	 * @brief Get look point with plane on same hight as transform component
	 * @param renderSystem 
	 * @param menuSystem
	 * @param transComp Object tranform component
	 * @param output Result point
	 */
	static void GetLookPoint(GameRenderSystem& renderSystem, GameMenuSystem& menuSystem, TransformComponent& transComp, btVector3 &output);
	
	/**
	 * @brief Compute nearest hit point with object or ground from camera point of view
	 * @param renderSystem
	 * @param menuSystem
	 * @param output
	 */
	static void GetCherryPickVector(GameRenderSystem& renderSystem, GameMenuSystem& menuSystem, btVector3& output);
	


};

#endif // __GAME_PHYSICS_UTILITIES__H__