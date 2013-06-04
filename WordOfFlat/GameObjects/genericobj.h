#ifndef __GENERIC_OBJECT_H__
#define __GENERIC_OBJECT_H__

#include "../GameObjectsSrv/gameobjdef.h"
#include "../GameObjectsSrv/gamedrawobj.h"



class GameGenericObject : public IGameObject {
private:
	GameObjectId m_objId;
	sf::Drawable* m_pDrawable;
	sf::Texture* m_pTexture;
public:
	GameGenericObject(){}
	
	void SetShape( sf::Drawable *pShape) {m_pDrawable = pShape;}
	void SetTexture( sf::Texture *pTexture) {m_pTexture = pTexture; }
	
	
public:
//from IGameObject
	virtual void SetSrvObj(IGameObjectSrv *pSrvObj);
	
	virtual void UpdateObject(const b2Transform &mat);

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