#ifndef __GAME_PROPERTY_COMPONENT_MANAGER_H__
#define __GAME_PROPERTY_COMPONENT_MANAGER_H__

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/gmanagerbase.h>


class PropertyComponent;

class PropertyCompManager : public GameManagerBase<PropertyComponent>
{
public:
	PropertyCompManager(GameLogger *pLogger, GameEntityManager *pEntityMgr);
	~PropertyCompManager();
	
	//////////////
	// Creators //
	//////////////
	
	/**
	 * \brief Create transform component from transform component definition
	 * \param compId component id ( identification of component )
	 * \param pTransComp New transform component
	 */
	GameErrorCode CreateComponent( wxDword compId, PropertyComponent *&pPropComp );
	
};

#endif // __GAME_PROPERTY_COMPONENT_MANAGER_H__