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
class IGameComponentMgr {
public:	
	virtual GameCompType GetType() = 0;
		
	virtual ~IGameComponentMgr() {}
		
};


#endif //__GAME_COMPONENT_INTERFACE_H__