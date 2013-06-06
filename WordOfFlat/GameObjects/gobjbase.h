#ifndef __GENERIC_OBJECT_H__
#define __GENERIC_OBJECT_H__

#include "../GameObjectsSrv/gameobjdef.h"
#include "../GameObjectsSrv/gameupdobj.h"

class IGameObjectSrv;

class GameObjectBase : public IGameObject, public sf::Drawable {
private:
	GameObjectId m_objId;
	IGameObjectSrv* m_pSrvObj;
	
public:
	GameObjectBase(){}
	void SetSrvObj(IGameObjectSrv *pSrvObj) { m_pSrvObj = pSrvObj;}
	void SetObjID(GameObjectId objID) { m_objId = objID;}
	
public:
//from IGameObject
	virtual GameObjectId GetObjID() { return m_objId;}
	virtual IGameObjectSrv* GetSrvObj() { return m_pSrvObj;}
	
	//virtual void UpdateObject(const b2Transform &mat);
	//virtual void UpdateObject(const b2Transform &mat, const GameObjStateStruct &state);
	
	virtual b2Vec2 GetMiddlePoint();
	virtual b2AABB GetAABB();
public:
// from IRefObject
	virtual void addRef();
	virtual wxInt32 release();
}


#endif //__GENERIC_OBJECT_H__