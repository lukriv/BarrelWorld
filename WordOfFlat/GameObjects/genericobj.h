#ifndef __GENERIC_OBJECT_H__
#define __GENERIC_OBJECT_H__

#include "../GameObjects/gamedrawobj.h"


class GameGenericObject : public IGameObject {
	typedef wxVector<sf::Shape*> ShapeListType;
private:
	ShapeListType m_shapeList;
	
public:
	GameGenericObject(){}
	
	GameErrorCode AddShape( sf::Shape *pShape);
	
	
public:
//from IGameObject
	virtual void SetSrvObj(IGameObjectSrv *pSrvObj);
	
	virtual void UpdateObject(const b2Transform &mat);
	virtual void UpdateObject(const b2Transform &mat);
	
	virtual void UpdatePart(const b2Transform &mat, wxDword index);
	virtual void UpdatePart(const b2Transform &mat, wxDword index);
	
	virtual b2Vec2 GetMiddlePoint();
	virtual b2Vec2 GetLLPoint();
	virtual b2Vec2 GetRUPoint();
		
	/*! \brief Draw an object
	 * 
	 * If object is not drawable, this method do nothing.
	 * 
	 * \retval true Drawing object was successful.
	 * \retval false Drawing object failed.
	 */
	virtual bool Draw();
}


#endif //__GENERIC_OBJECT_H__