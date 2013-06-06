#ifndef __GAME_SERVER_OBJECT_H__
#define __GAME_SERVER_OBJECT_H__

#include "../GameSystem/refobject.h"
#include "gameobjdef.h"
#include "gameupdobj.h"

class IGameObjectSrv : public IRefObject {
public:
	virtual GameObjectId GetObjId() = 0;
	virtual GameObjectType GetObjType() = 0;
	virtual wxDword GetObjFlags() = 0;
	virtual IGameObject* GetUpdObj() = 0;
	virtual b2Body* GetBody(wxDword idx) = 0;
	virtual wxDword GetBodyCount() = 0;
	virtual b2Body* GetLeadBody() = 0;
	virtual b2AABB& GetAABB() = 0;

	/*! \brief Do action on the object
	 * 
	 * Action are created by the events from update system (window events, network events).
	 * 
	 * \param action Action code
	 * \param pActionData Data nedded for action
	 */
	virtual void DoAction(GameActionCode action, void* pActionData ) = 0;
	
	/*! \brief Synchronizes object
	 * 
	 * Synchronizes object bodies with AABB box and update updateable object.
	 * 
	 * \retval FWG_NO_ERROR on success
	 * \retval other errorcode on fail
	 */
	GameErrorCode Synchronize() = 0;

	virtual ~IGameObjectSrv() {};
}

#endif //__GAME_SERVER_OBJECT_H__
