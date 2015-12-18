#ifndef __GAME_RENDER_POSITION_MANAGER_H__
#define __GAME_RENDER_POSITION_MANAGER_H__

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/gmanagerbase.h>

class GameRenderSystem;
class RenderPosition;
class TransformComponent;


class RenderPositionManager : public GameManagerBase<RenderPosition>
{
	GameRenderSystem *m_pRenderSystem;
public:
	RenderPositionManager(GameRenderSystem *pRenderSystem);
	~RenderPositionManager();
	
	//////////////
	// Creators //
	//////////////
	
	/**
	 * \brief Create render position component from render position component definition
	 * \param compId component id ( identification of component )
	 * \param pRenderComp New render position component
	 */
	GameErrorCode CreateComponent( wxDword compId, TransformComponent* trans, RenderPosition *&pRenderComp );

};

#endif // __GAME_RENDER_POSITION_MANAGER_H__
