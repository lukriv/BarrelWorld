#ifndef __GAME_COMPONENT_INTERFACE_H__
#define __GAME_COMPONENT_INTERFACE_H__

enum GameCompType {
	GAME_COMP_UNKNOWN 		= 0,
	GAME_COMP_RENDER 		= 1
};


/*!
 * \class IGameComponent
 * \author Lukas
 * \date 18.9.2013
 * \file gcomp.h
 * \brief Game component interface
 * 
 * Game component with part data of entity
 */
class IGameComponent : IRefObject {
public:
	virtual GameObjectID GetID() = 0;
	virtual GameCompType GetType() = 0;
		
	virtual ~IGameComponent() {}
};


/*!
 * \class IGameComponentMgr
 * \author Lukas
 * \date 18.9.2013
 * \file gcomp.h
 * \brief Game component manager interface
 * 
 * Aggregate components of one type
 */
class IGameComponentMgr : public IRefObject {
public:	
	virtual GameCompType GetType() = 0;
	
	virtual GameErrorCode AddComponent(IGameComponent *pComp) = 0;
	virtual void RemoveComponent(const GameObjectID &objID) = 0;
	virtual IGameComponent* GetComponent(const GameObjectID &objID) = 0;
	
	virtual ~IGameComponentMgr() {}
		
};

typedef std::map<GameObjectID, RefObjSmPtr<IGameComponent>> TComponentMap;
typedef std::pair<GameObjectID, RefObjSmPtr<IGameComponent>> TComponentMapItem;



#endif //__GAME_COMPONENT_INTERFACE_H__