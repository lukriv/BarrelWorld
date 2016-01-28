#ifndef __GAME_TRANSFORM_COMPONENT_MANAGER_H__
#define __GAME_TRANSFORM_COMPONENT_MANAGER_H__

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/gmanagerbase.h>


class TransformComponent;

class TransformCompManager : public GameManagerBase<TransformComponent>
{
public:
	TransformCompManager(GameLogger *pLogger, GameEntityManager *pEntityMgr);
	~TransformCompManager();
	
	//////////////
	// Creators //
	//////////////
	
	/**
	 * \brief Create transform component from transform component definition
	 * \param compId component id ( identification of component )
	 * \param pTransComp New transform component
	 */
	GameErrorCode CreateComponent( wxDword compId, TransformComponent *&pTransComp );
	
};

#endif // __GAME_TRANSFORM_COMPONENT_MANAGER_H__