#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

/*!
 * \class IGameComponentMgr
 * \author Lukas
 * \date 18.9.2013
 * \file gcomp.h
 * \brief Game component manager interface
 * 
 * Aggregate components of one type
 */
class GameComponentMgr {
public:	
	virtual GameCompType GetType() = 0;
		
	virtual ~GameComponentMgr() {}
		
};


#endif //__GAME_COMPONENT_MANAGER_H__