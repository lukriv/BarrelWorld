#ifndef __GAME_RENDER_OBJECT_H__
#define __GAME_RENDER_OBJECT_H__


#include <OGRE/OgreMovableObject.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

class RenderObject : public RefObjectImpl<IRefObject> 
{
private:
	Ogre::MovableObject* m_pObject;
public:
	RenderObject(Ogre::MovableObject* pObject) : m_pObject(pObject) {}
	~RenderObject();
	
	inline Ogre::MovableObject* GetMovableObject() { return m_pObject; }
	
	
	inline bool IsCameraType() 
	{
		return (m_pObject->getMovableType().compare("Camera"));
	}
	
	inline Ogre::Camera* GetCamera() 
	{ 
		return (IsCameraType()?reinterpret_cast<Ogre::Camera*>(m_pObject):nullptr); 
	}
	 
};


#endif //__GAME_RENDER_OBJECT_H__