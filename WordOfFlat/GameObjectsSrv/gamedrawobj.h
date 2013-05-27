#ifndef __GAME_DRAWABLE_OBJECT_H__
#define __GAME_DRAWABLE_OBJECT_H__

#include <b2Math.h>
#include "refobject.h"
#include "../GameObjectsSrv/gameobjdef.h"

class IGameObjectSrv;

class IGameObject : public IRefObject {
public:
	virtual void SetSrvObj(IGameObjectSrv *pSrvObj) = 0;
	
	virtual void UpdateObject(const b2Transform &mat) = 0;
	virtual void UpdateObject(const b2Transform &mat) = 0;
	
	virtual void UpdatePart(const b2Transform &mat, wxDword index) = 0;
	virtual void UpdatePart(const b2Transform &mat, wxDword index) = 0;
	
	virtual b2Vec2 GetMiddlePoint() = 0;
	virtual b2Vec2 GetLLPoint() = 0;
	virtual b2Vec2 GetRUPoint() = 0;
		
	/*! \brief Draw an object
	 * 
	 * If object is not drawable, this method do nothing.
	 * 
	 * \retval true Drawing object was successful.
	 * \retval false Drawing object failed.
	 */
	virtual bool Draw() = 0;
}


#endif //__GAME_DRAWABLE_OBJECT_H__