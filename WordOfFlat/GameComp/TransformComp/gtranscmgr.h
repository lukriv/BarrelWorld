#ifndef __GAME_TRANSFORM_COMPONENT_MANAGER_H__
#define __GAME_TRANSFORM_COMPONENT_MANAGER_H__

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameResHold/gdeftables.h>


class TransformComponent;

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
	
	/*!
	 * \brief Create empty transform component
	 * \return Pointer to newly created transform component (can be NULL)
	 */
	GameErrorCode CreateEmptyTransformComponent(TransformComponent *&pRenderComp);
	
	/**
	 * \brief Create transform component from transform component definition
	 * \param transCompDef
	 * \param pTransformComp
	 */
	GameErrorCode CreateTransformComponent(const TransformDef &transCompDef, TransformComponent *&pTransformComp);
	
};

#endif // __GAME_TRANSFORM_COMPONENT_MANAGER_H__