#ifndef __GAME_SERVER_OBJECT_IMPL_H__
#define __GAME_SERVER_OBJECT_IMPL_H__

#include "gamesrvobj.h"
#include "../GameSystem/glog.h"
#include "gameobjdef.h"

class GameObjectSrv : public IGameObjectSrv {
private:
	wxAtomicInt m_refCount;
	GameFlatWorldSrv *m_pParent;
	
	GameObjectType m_objType;
	GameObjectId m_objID;
	wxDword m_objFlags;
	
	wxDword m_leadIndex;
	wxVector<b2Body*> m_bodyVec;
	
	b2AABB m_AABB;
	
	IGameObject* m_pUpdObj;
public:

	virtual GameObjectId GetObjId() { return m_objID;}
	virtual GameObjectType GetObjType() { return m_objType;}
	virtual wxDword GetObjFlags() { return m_objFlags;}
	virtual IGameObject* GetUpdObj() { return m_pUpdObj;}
	virtual b2Body* GetBody(wxDword idx) { return m_bodyVec;}
	virtual wxDword GetBodyCount() { return static_cast<wxDword> (m_bodyVec.size()); }
	virtual b2Body* GetLeadBody() { return m_leadIndex;}
	virtual b2AABB& GetAABB() { return m_AABB; }

	virtual void DoAction(GameActionCode action, void* pActionData );
	
		/*! \brief Synchronizes object
	 * 
	 * Synchronizes object bodies with AABB box and update updateable object.
	 * 
	 * \retval FWG_NO_ERROR on success
	 * \retval other errorcode on fail
	 */
	GameErrorCode Synchronize();
public:
	virtual void addRef();
	virtual wxInt32 release();
public:
	GameObjectSrv(GameFlatWorldSrv* pParent) :m_refCount(1),
						m_pParent(pParent),
						m_objType(GAME_OBJECT_TYPE_UNKNOWN),
						m_objID(GAME_OBJECT_ID_UNKNOWN),
						m_objFlags(GAME_OBJECT_FLAG_NOTHING),
						m_leadIndex(0),
						m_pUpdObj(nullptr) {}
	
	
	void SetObjID(GameObjectId objID) { m_objID = objID;}
	virtual void SetUpdObj(IGameObject *pUpdObj);
	

	GameErrorCode AddBody( b2Body* pBody);
};


#endif //__GAME_SERVER_OBJECT_IMPL_H__