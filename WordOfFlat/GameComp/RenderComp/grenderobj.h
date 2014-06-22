#ifndef __GAME_RENDER_OBJECT_H__
#define __GAME_RENDER_OBJECT_H__


#include <OGRE/OgreMovableObject.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

class RenderObject : public RefObjectImpl<IRefObject> 
{
public:
	enum RenderObjectType {
		RENDER_OBJECT_TYPE_UNKNOWN 	= 0,
		RENDER_OBJECT_TYPE_ENTITY	= 1,
		RENDER_OBJECT_TYPE_CAMERA 	= 2,
		RENDER_OBJECT_TYPE_LIGHT 	= 3
	};
private:
	RenderObjectType m_type;
public:
	RenderObject(RenderObjectType type) : m_type(type) {}
	virtual ~RenderObject() {}
	
	inline RenderObjectType GetType() { return m_type; }
public:
	virtual Ogre::MovableObject* GetMovableObject() = 0;
};


#endif //__GAME_RENDER_OBJECT_H__