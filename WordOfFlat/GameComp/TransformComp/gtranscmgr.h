#ifndef __GAME_TRANSFORM_COMPONENT_MANAGER_H__
#define __GAME_TRANSFORM_COMPONENT_MANAGER_H__

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameResHold/gdeftables.h>


class TransformComponent;
class GameEntity;

class TransformCompManager
{
private:
	GameLoggerPtr m_spLogger;
	
public:
	TransformCompManager(GameLogger *pLogger);
	~TransformCompManager();
	
	//////////////
	// Creators //
	//////////////
	
	/**
	 * \brief Create transform component from transform component definition
	 * \param transCompDef
	 * \param pEntity transform component container
	 */
	GameErrorCode CreateTransformComponent( TransformComponent *&pTransComp, const btVector3& position, const btVector3& scale, const btQuaternion& rotation);
	
	GameErrorCode CreateTransformComponent( TransformComponent *&pTransComp );
	
};

#endif // __GAME_TRANSFORM_COMPONENT_MANAGER_H__